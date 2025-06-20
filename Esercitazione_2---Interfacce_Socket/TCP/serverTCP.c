#include "network.h"

int main(void) {
    int request;
    socketif_t socket;
    connection_t connection;
    int somma = 0;
    
    socket = createTCPServer(35000);                        //creazione socket
    if (socket < 0){
        printf("[SERVER] Errore di creazione del socket: %i\n", socket);
        return -1;
    }

    printf("[SERVER] Sono in attesa di richieste di connessione da qualche client\n");    
    connection = acceptConnection(socket);              //accetta connessione (three hand-shake)
    printf("[SERVER] Connessione instaurata\n");

    do{
        TCPReceive(connection, &request, sizeof(request));  //ascolto
        printf("[SERVER] Ho ricevuto la seguente richiesta dal client: %d\n", request);
        somma += request;
    }while(request != 0);

    printf("[SERVER] Invio la risposta al client\n");
    TCPSend(connection, &somma, sizeof(somma));         //risposta
    closeConnection(connection);                        //chiusura connessione
}

/*
1.INIZIO
2.Dichiarazione delle variabili tra cui la variabile socket, la variabile connessione e due
interi
3.Inizializzazione dell’interfaccia socket sulla porta 35000
4.SE c’è stato errore ALLORA
1.Scrivi "Errore nella creazione del socket"
5.ALTRIMENTI
1.Scrivi "[SERVER] Sono in attesa di richieste da qualche client"
2.Accetta la richiesta di connessione
3.Scrivi "[SERVER] Connessione instaurata"
4.Mettiti in attesa dell’arrivo di un messaggio TCP e metti il valore ricevuto in request
5.Scrivi "[SERVER] Ho ricevuto il seguente messaggio dal client"
6.Scrivi request
7.response=request+1
8.Invia response al client
9.Chiudi la connessione
6. FINE SE
FINE
*/