#include "network.h"

int main(void) {
    socketif_t socket;
    char response[]="Sono il server: ho ricevuto correttamente il tuo messaggio!\n";
    char request[MTU];
    char hostAddress[MAXADDRESSLEN];
    int port;
    socket=createUDPInterface(35000);       //porta di ascolto
    printf("%d", port);
    //ascolto infinito
    while(true){
        printf("[SERVER] Sono in attesa di richieste da qualche client\n");
        UDPReceive(socket, request, MTU, hostAddress, &port);
        printf("[SERVER] Ho ricevuto un messaggio da host/porta %s/%d\n", hostAddress, port);
        printf("[SERVER] Contenuto: %s\n", request);
        UDPSend(socket, response, strlen(response), hostAddress, port);
    }
}

/*
1.INIZIO
2.Dichiarazione delle variabili tra cui la variabile socket
3.Inizializzazione dell’interfaccia socket sulla porta 35000
4.Scrivi "[SERVER] Sono in attesa di richieste da qualche
client"
5.Mettiti in attesa dell’arrivo di un messaggio UDP
6.Scrivi "[SERVER] Ho ricevuto un messaggio da"
7.Scrivi host/porta del mittente e contenuto ricevuto
8.Invia a tale mittente tramite UDP il messaggio "Sono il
server: ho ricevuto correttamente il tuo messaggio!"
9.FINE
*/