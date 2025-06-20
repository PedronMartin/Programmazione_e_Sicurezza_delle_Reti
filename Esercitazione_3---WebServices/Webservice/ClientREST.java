import java.io.*; 
import java.net.*;

class ClientREST 
{    
    public static void main(String args[])
    {
        long startTime = System.currentTimeMillis();
        RESTAPI service1=new RESTAPI("127.0.0.1");

        if(args.length < 3)
            System.out.println("USAGE: java ClientREST tipofunzione op1 op2");
        else if(args[0].equals("calcola-somma"))
            System.out.println("Risultato: " + service1.calcolaSomma(Float.parseFloat(args[1]), Float.parseFloat(args[2])));
        else if(args[0].equals("primenumber")) {
            System.out.println("Risultato: ");
            service1.primenumber(Float.parseFloat(args[1]), Float.parseFloat(args[2]));
        }
        else
            System.out.println("Funzione non supportata");

        long endTime = System.currentTimeMillis();
        System.out.println("Tempo di esecuzione: " + (endTime - startTime)/1000 + " s");
    }
}

@SuppressWarnings("deprecation")
class RESTAPI
{
    String server;

    RESTAPI(String remoteServer)  {
        server = new String(remoteServer);
    }

    float calcolaSomma(float val1, float val2)  {

        URL u = null;
        float risultato=0;
        int i;

        try 
        { 
            u = new URL("http://"+server+":8000/calcola-somma?param1="+val1+"&param2="+val2);
            System.out.println("URL aperto: " + u);
        }
        catch (MalformedURLException e) 
        {
            System.out.println("URL errato: " + u);
        }

        try 
        {
            URLConnection c = u.openConnection();
            c.connect();
            BufferedReader b = new BufferedReader(new InputStreamReader(c.getInputStream()));
            System.out.println("Lettura dei dati...");
            String s;
            while( (s = b.readLine()) != null ) {
                System.out.println(s);
                if((i = s.indexOf("somma"))!=-1)
                    risultato = Float.parseFloat(s.substring(i+7));
            }
        }
        catch (IOException e) 
        {
            System.out.println(e.getMessage());
        }
    
        return (float)risultato;
    }

    void primenumber(float val1, float val2)  {

        URL u = null;

        try 
        { 
            u = new URL("http://"+server+":8000/primenumber?param1="+val1+"&param2="+val2);
        }
        catch (MalformedURLException e) 
        {
            System.out.println("URL errato: " + u);
        }

        try 
        {
            URLConnection c =  u.openConnection();
            c.connect();
            BufferedReader b = new BufferedReader(new InputStreamReader(c.getInputStream()));
            System.out.println("Lettura dei dati...");
            String s;
            while( (s = b.readLine()) != null ) {
                System.out.println(s);
            }
        }
        catch (IOException e) 
        {
            System.out.println(e.getMessage());
        }
     }
}

/* DOMANDA:
 Il fatto che il server sia fatto in C e il client in Java è un problema o un’opportunità? Perché?
*/

/* RISPOSTA:
Il server implementa una semplice versione della SOA, ossia fornisce un servizio che prescinde dal linguaggio di programmazione del client, dato che il tipo di risposta (JSON)
    è standard e può essere interpretato da qualsiasi linguaggio di programmazione.
*/
