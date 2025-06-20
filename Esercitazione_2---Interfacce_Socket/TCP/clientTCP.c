#include "network.h"

int main(void) {
    connection_t connection;
    int request;
    int result;
 	
    printf("[CLIENT] Creo una connessione logica col server\n");
    connection = createTCPConnection("localhost", 35000);               //creazione socket + three hand-shake

    if (connection < 0) {
        printf("[CLIENT] Errore nella connessione al server: %i\n", connection);
        return -1;
    }

    do{
    
        printf("[CLIENT] Inserisci un numero intero:\n");
        scanf("%d", &request);
        printf("[CLIENT] Invio richiesta con numero al server\n");
        TCPSend(connection, &request, sizeof(request));

    }while(request != 0);

    TCPReceive(connection, &result, sizeof(result));
    printf("[CLIENT] Ho ricevuto la seguente risposta dal server: %d\n", result);
    closeConnection(connection);                                        //chiusura TCP connessione
}

/*
1.INIZIO
2.Dichiarazione delle variabili tra cui la variabile connessione e due interi
3.Instaurazione della connessione verso il server destinatario avente nome "localhost" e
porta 35000
4.SE c’è stato errore ALLORA
1.Scrivi "Errore nella connessione al server"
5.ALTRIMENTI
1.Scrivi "Inserisci un numero intero:"
2.Leggi request da tastiera
3.Invia request tramite la connessione TCP
4.Mettiti in attesa dell’arrivo di un messaggio dalla connessione TCP e metti il valore ricevuto in
response
5.Scrivi "[CLIENT] Ho ricevuto un messaggio dal server"
6.Scrivi response
7.Chiudi la connessione
6.FINE SE
7.FINE
*/
