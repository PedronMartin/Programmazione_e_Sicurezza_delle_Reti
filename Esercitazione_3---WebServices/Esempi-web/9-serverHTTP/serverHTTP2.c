#include "network.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//dimensione massima file html
#define MAX_FILE_SIZE 8192

int main(){

    //dichiarazioni
    socketif_t sockfd;
    FILE* connfd;
    int res, i;
    long length = 0;
    char request[MTU];
    char method[10];
    char filename[256];
    char line[256];
    char fullResponse[256 + MAX_FILE_SIZE];         //header HTTP + contenuto massimo

    //creazione socket con gestione errore
    sockfd = createTCPServer(8000);
    if(sockfd < 0) {
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }

    //server sempre in ascolto
    while(true) {

        //attesa e accettazione client (funz. bloccante)
        connfd = acceptConnectionFD(sockfd);

        //prima riga della richiesta. Se nulla, gestisco l'errore e passo al prossimo client
        if(fgets(request, sizeof(request), connfd) == NULL) {
            perror("[SERVER] Errore nella lettura della richiesta");
            fclose(connfd);
            continue;
        }

        //estrazzione prima riga per ottenere metodo e filename
        if(sscanf(request, "%s %s HTTP/1.1", method, filename) == 2)
            printf("[SERVER] Metodo: %s, File richiesto: %s\n", method, filename);
        else
            printf("[SERVER] Errore nell'analisi della prima riga della richiesta\n");

        //continuo ad estrarre per ottenere il Content-Length
        while(fgets(line, sizeof(line), connfd) != NULL && strcmp(line, "\r\n") != 0){
            if(strstr(line, "Content-Length:") != NULL){
                length = atol(line + 15);
                printf("[SERVER] Content-Length: %ld\n", length);
            }
        }

        //rimuovo la '/' iniziale dal nome del file
        if(filename[0] == '/')
            memmove(filename, filename + 1, strlen(filename));
        printf("\n[SERVER] FILENAME: %s\n", filename);

        //cerco e apro il file richiesto. Se non esiste, invio errore 404 e passo al prossimo client
        FILE *requestedFile = fopen(filename, "rb");
        if(requestedFile == NULL){
            perror("[SERVER] Errore nell'apertura del file");
            char errorResponse[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            fputs(errorResponse, connfd);
            fclose(connfd);
            continue;
        }        
        printf("[SERVER] File aperto correttamente: %s\n", filename);

        //ottengo la dimensione del file
        fseek(requestedFile, 0, SEEK_END);
        long fileSize = ftell(requestedFile);
        fseek(requestedFile, 0, SEEK_SET);

        //alloco il buffer per contenere l'intero file
        char *fileBuffer = (char *)malloc(fileSize + 1); //+1 per il terminatore null (anche se per HTML non è strettamente necessario)
        if(fileBuffer == NULL){
            perror("[SERVER] Errore nell'allocazione della memoria per il file");
            fclose(requestedFile);
            char errorResponse[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
            fputs(errorResponse, connfd);
            continue;
        }

        //leggo l'intero contenuto del file nel buffer in un colpo solo. 
        //il contenuto del file è fileBuffer e la sua lunghezza è bytesRead
        size_t bytesRead = fread(fileBuffer, 1, fileSize, requestedFile);
        fclose(requestedFile);
        fileBuffer[bytesRead] = '\0';
        printf("[SERVER] File letto. Bytes letti: %zu\n", bytesRead);

        //stringa fissa dell'header HTTP di base: risposta 200 positiva, tipo di contenuto HTML e charset dei caratteri utf-8
        char basicHeader[256];
        snprintf(basicHeader, sizeof(basicHeader),
            "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %zu\r\n\r\n", bytesRead);
        size_t headerLength = strlen(basicHeader);

        //verifica che il contenuto del file non superi la dimensione massima
        if (bytesRead > MAX_FILE_SIZE) {
            printf("[SERVER] Errore: File troppo grande per il buffer statico.\n");
            fclose(requestedFile);
            char errorResponse[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
            fputs(errorResponse, connfd);
            continue;
        }

        //compongo risposta completa: header + file
        strcpy(fullResponse, basicHeader);
        memcpy(fullResponse + headerLength, fileBuffer, bytesRead);

        //calcolo lunghezza totale della risposta
        size_t responseLength = headerLength + bytesRead;

        //invio la risposta al client
        fputs(fullResponse, connfd);

        //libero memoria e chiudo il file
        free(fileBuffer);
        fclose(connfd);
        printf("\n\n[SERVER] sessione HTTP completata\n\n");
    }

    //chiudo la socket
    closeConnection(sockfd);
    return 0;
}
