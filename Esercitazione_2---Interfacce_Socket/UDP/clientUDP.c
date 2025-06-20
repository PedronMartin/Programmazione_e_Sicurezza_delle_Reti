//libreria funzioni di socket
#include "network.h"

int main(void) {
    socketif_t socket;
    char request[]="Ciao sono il client!\n";
    char response[MTU];
    char hostAddress[MAXADDRESSLEN];
    int port;
    socket = createUDPInterface(20000); //ascolta su porta 20000 per risposta
    UDPSend(socket, request, strlen(request), "localhost", 35000); //invia a server su porta 35000
    UDPReceive(socket, response, MTU, hostAddress, &port);
    printf("[CLIENT] Ho ricevuto un messaggio da host/porta %s/%d\n", hostAddress, port);
    printf("[CLIENT] Contenuto: %s\n", response);
}

/*
PSEUDOCODICE:
1.INIZIO
2.Dichiarazione delle variabili tra cui la variabile socket
3.Inizializzazione dell’interfaccia socket sulla porta 20000
4.Invia tramite UDP il messaggio "Ciao sono il client!" al
destinatario avente indirizzo IP "127.0.0.1" e porta 35000
5.Mettiti in attesa dell’arrivo di un messaggio UDP
6.Scrivi "[CLIENT] Ho ricevuto un messaggio da"
7.Scrivi host/porta del mittente e contenuto ricevuto
8.FINE
*/

/*
Compilazione:
gcc network.c client.c -o EXEC1
gcc network.c server.c -o EXEC2
esegui in due terminali diversi gli EXEC
*/