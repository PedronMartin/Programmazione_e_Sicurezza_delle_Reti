#include "network.h"

float calcolaSomma(float val1, float val2) {
   return (val1 + val2);
}

void primeFromTo(float val1, float val2, char **result) {
    *result = malloc(1000000);
    if (*result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    int flag;
    for (int i = val1; i <= val2; i++) {

        if (i == 1 || i == 0)
            continue;
 
        flag = 1;
 
        for (int j = 2; j <= i / 2; ++j) {
            if (i % j == 0) {
                flag = 0;
                break;
            }
        }

        if (flag == 1)
            sprintf(*result + strlen(*result), "%d, ", i);
    }
    sprintf(*result + strlen(*result) - 2, "\0");
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
        if(strstr(url, "calcola-somma")==NULL && strstr(url, "primenumber")==NULL)
            fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    Funzione non riconosciuta!\r\n}\r\n");
        else {

            //dichiarazioni ed estrazione variabili generiche (skeleton)
            char *function, *op1, *op2;
            float somma, val1, val2;
            function = strtok(url, "?&");
            op1 = strtok(NULL, "?&");
            op2 = strtok(NULL, "?&");
            strtok(op1,"=");

            //conversione dei parametri estratti generica
            val1 = atof(strtok(NULL,"="));
            strtok(op2,"=");
            val2 = atof(strtok(NULL,"="));

            if(strstr(url, "calcola-somma")!=NULL) {
                printf("Chiamata a funzione sommatrice\n");
                
                //chiamata alla funzione locale
                somma = calcolaSomma(val1, val2);
                
                //skeleton: codifica (serializzazione) del risultato
                fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    \"somma\":%f\r\n}\r\n", somma);
            }
            else if(strstr(url, "primenumber")!=NULL) {
                printf("Chiamata a funzione primenumber\n");

                //chiamata alla funzione locale
                char *result;
                primeFromTo(val1, val2, &result);

                //skeleton: codifica (serializzazione) del risultato
                fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    \"result\":%s\r\n}\r\n", result);
            }
        }
        
        //chiusura connessione
        fclose(connfd);
        printf("\n\n[SERVER] sessione HTTP completata\n\n");
    }

    closeConnection(sockfd);
    return 0;
}

/* DOMANDA:
Provare il client con il calcolo dei numeri primi compresi nell’intervallo [1,
1000000]
– Quanto tempo ci mette?
– E’ stato necessario tradurre l’algoritmo dei numeri primi in Java? Perché?
*/

/* RISPOSTA:
- ci mette circa un minuto;
- No, non è stato necessario tradurre l'algoritmo dei numeri primi in Java, perché è il server che implementa questo servizio.
    In java ho solo aggiunto la gestione del diverso tipo di risposta (stringa in questo caso).
*/
