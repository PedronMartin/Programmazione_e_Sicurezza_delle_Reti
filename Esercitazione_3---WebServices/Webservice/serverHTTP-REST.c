#include "network.h"

float calcolaSomma(float val1, float val2) {
   return (val1 + val2);
}

int main(){
    socketif_t sockfd;
    FILE* connfd;
    int res, i;
    long length=0;
    char request[MTU], url[MTU], method[10], c;
    
    //creazione connessione
    sockfd = createTCPServer(8000);
    if(sockfd < 0){
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }
    
    //attendo e servo tutte le richieste del client
    while(true) {
        connfd = acceptConnectionFD(sockfd);
        
        //analizzo metodo HTTP, url e content-length
        fgets(request, sizeof(request), connfd);
        strcpy(method,strtok(request, " "));
        strcpy(url,strtok(NULL, " "));
        while(request[0]!='\r') {
            fgets(request, sizeof(request), connfd);
            if(strstr(request, "Content-Length:")!=NULL)
                length = atol(request+15);
        }
        
        //se il metodo è POST leggo il body
        if(strcmp(method, "POST")==0)
            for(i=0; i<length; i++)
                c = fgetc(connfd);
        
        //analizzo l'url
        if(strstr(url, "calcola-somma")==NULL)
            fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    Funzione non riconosciuta!\r\n}\r\n");

        else {
            printf("Chiamata a funzione sommatrice\n");
            
            char *function, *op1, *op2;
            float somma, val1, val2;

            //skeleton: decodifica (de-serializzazione) dei parametri
            function = strtok(url, "?&");
            op1 = strtok(NULL, "?&");
            op2 = strtok(NULL, "?&");
            strtok(op1,"=");

            //conversione dei parametri estratti
            val1 = atof(strtok(NULL,"="));
            strtok(op2,"=");
            val2 = atof(strtok(NULL,"="));
            
            //chiamata alla funzione locale
            somma = calcolaSomma(val1, val2);
            
            //skeleton: codifica (serializzazione) del risultato
            fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    \"somma\":%f\r\n}\r\n", somma);
        }
        
        //chiusura connessione
        fclose(connfd);
        printf("\n\n[SERVER] sessione HTTP completata\n\n");
    }

    closeConnection(sockfd);
    return 0;
}

/* DOMANDE:
– Che parametri devo passare in linea di comando?
– Cosa si può vedere analizzando lo scambio di dati tramite Wireshark?
– Qual è la signature della funzione calcolaSomma() sul server e sul client? Perché ha senso che siano uguali?
*/

/* RISPOSTE:
- nome funzione, op1 e op2
- WSH mette a lato una piccola barra nera che mostra dove inizia e finisce una determinata connessione, dato che intorno è pieno di pacchetti DNS e ICMP che non riguardano strettamente la connesssione.
    Poi vediamo il three-hand shake. Continua con il client che invia una richiesta GET al server, dove vediamo chiaramente lo Stub dei dati su cui fare il calcolo.
    Il server conferma di aver ricevuto con un ACK, e poi risponde con un JSON contenente la somma dei due numeri. Questo avviene in due PDU diversi (TCP segment of a reassemble PDU).
    Poi vediamo il riassemblamento dei PDU nella risposta HTTP 200 OK, e infine la chiusura della connessione TCP con la flag FYN;
- sia sul client che sul server, la signature della funzione calcolaSomma() è float calcolaSomma(float val1, float val2).
    Devono essere uguali perché definiscono il contratto dell'API, permettendo al client di invocare la funzione remota come se fosse locale.
*/
