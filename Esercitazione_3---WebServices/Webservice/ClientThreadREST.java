import java.io.*; 
import java.net.*;
import java.util.ArrayList;
import java.util.List;

class ClientThreadREST{    

    public static void main(String args[]){
        if(args.length < 3)
            System.out.println("USAGE: java ClientREST tipofunzione op1 op2");
        else  {
            threads_result_collector.setStartTime();
            RESTAPI service1=new RESTAPI("127.0.0.1:8000", args[0], args[1], args[2], 0);
            RESTAPI service2=new RESTAPI("127.0.0.1:8001", args[0], args[1], args[2], 1);
            RESTAPI service3=new RESTAPI("127.0.0.1:8002", args[0], args[1], args[2], 2);
            service1.start();
            service2.start();
            service3.start();
        }
    }

}

@SuppressWarnings("deprecation")
class RESTAPI extends Thread{

    String server, service, param1, param2;
    int numThread;

    public void run()   {
        if(service.equals("calcola-somma")){
            float res = calcolaSomma(Float.parseFloat(param1), Float.parseFloat(param2));
            if(numThread == 0){
                System.out.println("\nRisultato: ");
                System.out.println(res + "\t//nascosto risultato di thread 2 e 3 in quanto ripetitive per somma");
            }
        }
        else if(service.equals("primenumber")){
            String result = primenumber().toString();
            //tolgo da ogni thread result il '[' iniziale skippando il primo carattere (replaceAll non funziona con l'indice 0!!!)
            result = result.substring(1, result.length() - 1);
            threads_result_collector.memorizeResult(result, numThread);
            threads_result_collector.printResult(numThread);
        }
        else
            System.out.println("Servizio non disponibile!");
    }

    RESTAPI(String remoteServer, String srvc, String p1, String p2, int numThread)  {
        int temp1, temp2;
        server = new String(remoteServer);
        service = new String(srvc);
        this.numThread = numThread;
        if(service.equals("calcola-somma")){
            param1 = new String(p1);
            param2 = new String(p2);
        }
        else{
            try {
                temp1 = Integer.parseInt(p1);
                temp2 = Integer.parseInt(p2);

                int range = (temp2 - temp1)/3;
                int min = temp1 + (range * numThread);
                int max = temp1 + (range * (numThread + 1));

                param1 = "" + min;
                param2 = "" + max;
            } catch (NumberFormatException e) {
                System.out.println("I parametri devono essere numeri validi.");
                System.exit(1);
            }
        }
    }

    synchronized float calcolaSomma(float val1, float val2)  {

        URL u = null;
        float risultato=0;
        int i;

        try { 
            u = new URL("http://"+server+"/calcola-somma?param1="+val1+"&param2="+val2);
            //System.out.println("URL aperto: " + u);
        }
        catch (MalformedURLException e) {
            System.out.println("URL errato: " + u);
        }

        try {
            URLConnection c = u.openConnection();
            c.connect();
            BufferedReader b = new BufferedReader(new InputStreamReader(c.getInputStream()));
            String s;
            while((s = b.readLine()) != null){
                if((i = s.indexOf("somma"))!=-1)
                    risultato = Float.parseFloat(s.substring(i+7));
            }
        }
        catch (IOException e) {
            System.out.println(e.getMessage());
        }
    
        return (float)risultato;
    }

    synchronized List<Integer> primenumber()  {

        URL u = null;
        List<Integer> risultato = new ArrayList<>();
        int i;

        try { 
            u = new URL("http://"+server+"/primenumber?param1="+param1+"&param2="+param2);
            //System.out.println("URL aperto: " + u);
        }
        catch (MalformedURLException e) {
            System.out.println("URL errato: " + u);
        }

        try {
            URLConnection c = u.openConnection();
            c.connect();
            BufferedReader b = new BufferedReader(new InputStreamReader(c.getInputStream()));
            String s;
            while( (s = b.readLine()) != null ) {
                //System.out.println(s);
                /*if((i = s.indexOf("numero"))!=-1) {
                    String numStr = s.substring(i+8).trim();
                    */
                    String[] parts = s.split(",");
                    for(String part : parts) {
                        risultato.add(Integer.parseInt(part.trim()));
                    }
                //}
            }
        }
        catch (IOException e) {
            System.out.println(e.getMessage());
        }
    
        return risultato;
    }    

}

//statica perchè vogliamo che valga per tutte le thread
public class threads_result_collector {

    //variabili globali valide per ogni thread
    private static boolean flag1 = false, flag2 = false, flag3 = false, alreadyDone = false;
    private static String res1, res2, res3;
    private static long startTime, endTime;
    private static int lastThread;

    public static void setStartTime(){
        startTime = System.currentTimeMillis();
    }

    public static void setEndTime(){
        endTime = System.currentTimeMillis();
    }

    public static void memorizeResult(String risultato, int numThread){

        switch(numThread){
            case 0: res1 = risultato;
                    flag1 = true;
                    break;
            case 1: res2 = risultato;
                    flag2 = true;
                    break;
            case 2: res3 = risultato;
                    flag3 = true;
                    break;
        }

        //viene sovvrascritto fino all'ultima thread a terminare il lavoro
        lastThread = numThread;
    }

    static public void printResult(int numThread){
        if(flag1 && flag2 && flag3 && lastThread == numThread){
            alreadyDone = true;
            String result = res1 + ", " + res2 + ", " + res3;
            result.replaceAll("]", "");
            //scrittura in file
            try (BufferedWriter writer = new BufferedWriter(new FileWriter("File_output", false))) {
                writer.write(result);
                writer.newLine();
            } catch (IOException e) {
                System.out.println("Errore nella scrittura del file: " + e.getMessage());
            }
            System.out.println("---> Risultato dentro file 'File_output'");
            setEndTime();
            System.out.println("Tempo di esecuzione: " + (endTime - startTime)/1000 + " s");
        }
        else
            return;
    }
}

/*
il problema principale è che, anche avendo più thread, se tutte puntano a localhost:8000, la CPU del server sarà comunque l'unico operativo. 
Per guadagnare in prestazioni, abbiamo bisogno di distribuire il carico su più server (virtuali nel nostro caso). I server vengono quindi eseguiti più volte parallelamente
e su porte diverse (dato che siamo sulla stessa macchina e non è possibile occupare la stessa porta più volte).
*/

/*DOMANDE:
Si trovi un modo efficiente, sfruttando diversi server in rete, per calcolare il numeri primi tra 1 e
1000000
–Di quanto migliorano le prestazioni?
–Devo modificare anche il codice del server?
*/

/* RISPOSTE:
Per calcolare i numeri primi tra 1 e 1.000.000, il codice è stato modificato per suddividere l'intervallo in tre parti, ognuna gestita da un thread diverso.
Le prestazioni migliorano notevolmente, poiché ogni thread calcola i numeri primi in un intervallo specifico, riducendo il tempo totale di esecuzione rispetto a un singolo thread che calcola l'intero intervallo.
Se non in qualche accortezza come l'apertura di porte diverse, e il maggiore spazio di heap allocabile per i tanti numeri primi, il codice del server non richiede modifiche significative.

Tempo intorno ai 35 secondi.
*/