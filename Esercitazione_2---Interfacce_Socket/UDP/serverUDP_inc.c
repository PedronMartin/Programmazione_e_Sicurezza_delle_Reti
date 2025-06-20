#include "network.h"

int main(void) {
    int request;
    socketif_t socket;
    char hostAddress[MAXADDRESSLEN];
    int port;
    int somma = 0;
    
    socket = createUDPInterface(35000);

    do{
    
        printf("[SERVER] Sono in attesa di richieste da qualche client\n");
        
        UDPReceive(socket, &request, sizeof(request), hostAddress, &port);
        
        printf("[SERVER] Ho ricevuto un messaggio da host/porta %s/%d\n", hostAddress, port);
        
        printf("[SERVER] Contenuto: %d\n", request);
        
        somma += request;

    }while(request != 0);

    printf("%d", somma);

    UDPSend(socket, &somma, sizeof(somma), hostAddress, port);

}

//server che prende numero ricevuto da client (inserito da utente) incrementa 1 e ritorna indietro
