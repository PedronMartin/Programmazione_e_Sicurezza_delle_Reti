#include "network.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 256
#define BUFFER_SIZE 4096

int main() {
    char *HTMLResponse = "HTTP/1.1 200 OK\r\n\r\n<html><head><title>Upload Completato</title></head><body>File caricato con successo.</body></html>\r\n";
    char *HTMLGETResponse = "HTTP/1.1 200 OK\r\n\r\n<html><head><title>Simple GET</title></head><body>HTTP GET base!</body></html>\r\n";
    socketif_t sockfd;
    FILE* connfd;
    int res, i;
    long length = 0;
    char request[MTU], method[10], c;
    char filename[MAX_FILENAME_LENGTH] = "uploaded_server";
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    FILE *uploadedFile = NULL;

    //creazione socket
    sockfd = createTCPServer(8000);
    if(sockfd < 0){
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }
    printf("[SERVER] Server HTTP in ascolto sulla porta 8000...\n");

    //server in ascolto continuo
    while(true){

        //attesa client
        connfd = acceptConnectionFD(sockfd);
        if(connfd == NULL){
            printf("[SERVER] Errore nell'accettare la connessione.\n");
            continue;
        }

        //leggo header richiesta
        fgets(request, sizeof(request), connfd);
        strcpy(method,strtok(request," "));
        while(request[0]!='\r') {
            sleep(0.1);
            fgets(request, sizeof(request), connfd);
            if(strstr(request, "Content-Length:")!=NULL)
                length = atol(request+15);
        }

        //lettura payload
        if(strcmp(method, "POST") == 0 && length > 0){
            printf("[SERVER] Ricevuta richiesta POST con Content-Length: %ld\n", length);
            FILE *uploadedFile = fopen(filename, "wb");
            if(uploadedFile != NULL){
                
                //leggo il payload
                for(i = 0; i < length; i++){
                    c = fgetc(connfd);
                    fputc(c, uploadedFile);
                }
                
                printf("[SERVER] File '%s' salvato con successo (%ld bytes).\n", filename, length);
                fclose(uploadedFile);
                fputs(HTMLResponse, connfd);
                fclose(connfd);
            }
            //risposta di errore
            else{
                printf("[SERVER] Errore nell'aprire il file '%s' per la scrittura.\n", filename);
                char *errorResponse = "HTTP/1.1 500 Internal Server Error\r\n\r\nErrore nel salvataggio del file.\r\n";
                fputs(errorResponse, connfd);
            }
        }
        //se non è un POST o non c'è Content-Length, invia la risposta standard (per eventuali GET)
        else{
            fputs(HTMLGETResponse, connfd);
            //chiudo la connessione
            printf("\n[SERVER] Sessione HTTP completata.\n\n");
        }

    }

    //chiudo la socket
    closeConnection(sockfd);
    return 0;
}