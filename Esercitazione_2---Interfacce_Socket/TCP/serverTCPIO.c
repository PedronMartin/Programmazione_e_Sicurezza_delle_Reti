#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {

    //dichiarazioni
    char nome_file[256];
    char byte;
    int file_fd;

    //server TCP con gestione dell'errore
    socketif_t server = createTCPServer(35000);
    if(server < 0) {
        perror("Errore nella creazione del server");
        return -1;
    }
    printf("Attesa connessione di un client\n");

    while(true) {

        //accetto connessione del client
        connection_t conn = acceptConnection(server);
        if(conn < 0) {
            perror("Errore nella accettazione della connessione");
            continue;
        }

        //ricevo nome del file
        int len = TCPReceive(conn, nome_file, sizeof(nome_file) - 1);
        if(len <= 0) {
            perror("Errore nella ricezione del nome file");
            close(conn);
            continue;
        }

        // rimuove newline se presente
        nome_file[len] = '\0';
        nome_file[strcspn(nome_file, "\n")] = 0;
        //strcpsn ritorna l'indice della prima occorrenza del carattere specificato, in questo caso "\n"

        printf("[SERVER] Richiesto file: %s\n", nome_file);

        //apertura file richiesto (con gestione errore)
        file_fd = open(nome_file, O_RDONLY);
        if(file_fd < 0) {
            perror("[SERVER] Errore apertura file");
            close(conn);
            continue;
        }

        //invia il file un byte alla volta
        while(read(file_fd, &byte, 1) == 1) {
            TCPSend(conn, &byte, 1);
        }

        //messaggio di termine e chiusura
        printf("[SERVER] File inviato.\n");
        close(file_fd);
        closeConnection(conn);
    }

    return 0;
}
