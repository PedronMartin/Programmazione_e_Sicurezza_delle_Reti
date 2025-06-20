#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    //dichiarazioni
    char nome_file[256];
    char byte;
    connection_t conn;

    //creazione socket TCP
    conn = createTCPConnection("localhost", 35000);
    if(conn < 0) {
        perror("Errore nella connessione al server");
        return 1;
    }

    //richiesta utente
    printf("Inserisci il nome del file da richiedere: ");
    fgets(nome_file, sizeof(nome_file), stdin);

    //invia il nome file al server
    TCPSend(conn, nome_file, strlen(nome_file));

    //riceve e stampa su terminale un byte alla volta
    while(TCPReceive(conn, &byte, 1) == 1) {
        write(1, &byte, 1);
    }

    printf("\n");

    //chiusura connessione
    closeConnection(conn);
    return 0;
}
