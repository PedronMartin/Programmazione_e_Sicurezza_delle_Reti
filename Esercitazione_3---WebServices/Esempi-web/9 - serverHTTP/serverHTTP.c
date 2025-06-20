#include "network.h"

int main(){
    char *HTMLResponse = "HTTP/1.1 200 OK\r\n\r\n<html><head><title>An Example Page</title></head><body>Hello World, this is a very simple HTML document.</body></html>\r\n";
    socketif_t sockfd;
    FILE* connfd;
    int res, i;
    long length=0;
    char request[MTU], method[10], c;
    
    sockfd = createTCPServer(8000);
    if (sockfd < 0){
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }
    
    while(true) {
        connfd = acceptConnectionFD(sockfd);
        
        fgets(request, sizeof(request), connfd);
        printf("%s", request);
        strcpy(method,strtok(request," "));
        while(request[0]!='\r') {
            fgets(request, sizeof(request), connfd);
            printf("%s", request);
            if(strstr(request, "Content-Length:")!=NULL)  {
                length = atol(request+15);
                //printf("length %ld\n", length);
            }
        }
        
        if(strcmp(method, "POST")==0)  {
            for(i=0; i<length; i++)  {
                c = fgetc(connfd);
                printf("%c", c);
            }
        }
        
        fputs(HTMLResponse, connfd);
        fclose(connfd);
                
        printf("\n\n[SERVER] sessione HTTP completata\n\n");
    }
    
    closeConnection(sockfd);
    return 0;
}

// domande riguardo questo codice:

/*
1) Analizzare il codice: il codice contiene una stringa con un messaggio HTTP di risposta positiva, con all'interno il codice HTML di una pagina da visualizzare.
    Tramite la libreria network.c viene creata una connessione TCP sulla porta 8000 di tipo server che rimane in ascolto di un eventuale client (while(true)).
    Ne estrae il metodo (GET, POST, ecc.), legge eventuali URL, e nel caso di un POST legge anche il corpo della richiesta. Questo nella prima riga della richiesta HTTP usando fgets.

    Estrae il metodo HTTP (solo il primo "token" separato da spazio) usando strtok.
    Legge le successive righe dell'header HTTP fino a trovare una riga vuota (\r). Durante la lettura degli header, cerca la riga Content-Length: per memorizzare la lunghezza del corpo della richiesta nel caso di un POST,
    stampandolo sul terminale.
    
    Dopo aver eseguito in terminale l'eseguibile, il server è aperto e in ascolto. Basta quindi simulare un client collegandosi al localhost sulla porta 8000 da qualsiasi browser.
    Il server risponde come detto in base al tipo di richiesta. Nel caso base di aprire localhost e basta, la richiesta è una semplice GET, per tanto viene visualizzata la pagina HTML statica visibile all'inizio del codice.

2) Aprire secondo terminale: come nell'esercitazione 2, il SO crea un binding tra PID processo e porta diciamo, associando quindi queste due entità.
    Provando ad aprire un secondo server in contemporanea, il SO vede subito che la porta è già utilizzata e rifiuta la richiesta.
    Da terminale si viene subito mandati alla riga di richiesta successiva della bash; questa parte di output probabilmente è gestita dalla libreria network.c;

3) Aprire 127.0.0.1 e localhost: sono la stessa cosa. Il SO interpreta localhost come un dominio noto, e vi associa l'IP 127.0.0.1;
    Di fatto nel terminale, quando compaiono tutte le informazioni della richiesta HTTP del client, viene mostrato 'localhost' in entrambi i casi.

NB: TENERE APERTO SERVER PER FARE ESEMPIO 10, 11, 12!!!

*/

