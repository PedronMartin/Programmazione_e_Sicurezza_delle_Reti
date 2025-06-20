#include "network.h"

int main(void) {
    int request;
    int response;
    socketif_t socket;
    char hostAddress[MAXADDRESSLEN];
    int port;
    
    socket = createUDPInterface(20000);

    do{
        
        printf("Inserisci un numero intero (inviare 0 per terminare):\n");
        
        scanf("%d", &request);
        
        UDPSend(socket, &request, sizeof(request), "localhost", 35000);

    }while(request != 0);

    UDPReceive(socket, &response, sizeof(response), hostAddress, &port);

    printf("[CLIENT] Ho ricevuto un messaggio da host/porta %s/%d\n", hostAddress, port);
        
    printf("[CLIENT] Contenuto: %d\n", response);
}

//questo codice è un client UDP che invia un numero intero inserito dall'utente al server e riceve lo stesso numero incrementato di uno

