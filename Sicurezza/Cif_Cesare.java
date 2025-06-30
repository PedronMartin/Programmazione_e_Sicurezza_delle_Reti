import java.util.Scanner;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Cif_Cesare {

    public static final String ANSI_GREEN = "\u001B[32m"; // Codice per il colore verde
    public static final String ANSI_RESET = "\u001B[0m";  // Codice per resettare il colore al default

    public static String cifratura(String testo, int chiave) {

        String result = new String();
        for(int i = 0; i < testo.length(); i++) {
            char c = testo.charAt(i);
            if(Character.isLetter(c)) {
                //gestione delle lettere minuscole e maiuscole
                if(Character.isLowerCase(c)) {
                    //calcolo della nuova lettera con la chiave
                    c += chiave;
                    //se supera 'z', torna all'inizio dell'alfabeto
                    if (c > 'z') c -= 26;
                }
                else {
                    c += chiave;
                    if (c > 'Z') c -= 26;
                }
            }
            result += c;
        }
        
        //stampa
        return result;
    }

    public static String decifratura(String testo, int chiave) {
        return cifratura(testo, 26 - chiave);
    }

    public static void main(String[] args) {

        // --- IMPOSTA IL COLORE VERDE PER TUTTA L'ESECUZIONE ---
        System.out.print(ANSI_GREEN);

        System.out.print("\033[H\033[2J");
        System.out.flush();
        System.out.println("Benvenuto nel programma di cifratura.\nSelezionare un'opzione:");
        System.out.println("1. Cifra un testo");
        System.out.println("2. Decifra un testo");
        System.out.println("3. Esci");
        Scanner scanner = new Scanner(System.in);
        int scelta = scanner.nextInt();
        System.out.print("\033[H\033[2J");
        System.out.flush();
        
        switch (scelta) {
            case 1:
                //pulisci terminale
                System.out.print("\033[H\033[2J");
                System.out.flush();

                System.out.println("Benvenuto nel programma di cifratura.\nSelezionare un'algoritmo di cifratura:");
                System.out.println("1. Cifratura di Cesare");
                System.out.println("2. Esci");

                int algoritmo = scanner.nextInt();
                switch (algoritmo) {
                    case 1:
                        System.out.println("Hai scelto la Cifratura di Cesare.");
                        //pulisci schermo
                        System.out.print("\033[H\033[2J");
                        System.out.flush();
                        break;
                    case 2:
                        System.out.println("Uscita dal programma.");
                        return;
                    default:
                        System.out.println("Algoritmo non valido.");    //va implementata la ciclatura per richiedere nuovamente l'algoritmo
                        return;
                }

                System.out.print("\033[H\033[2J");
                System.out.flush();

                //richiesta testo da cifrare
                System.out.println("Inserisci il testo da cifrare:");
                scanner.nextLine(); //consuma il newline rimasto
                String testoCifra = scanner.nextLine();
                
                //richiesta chiave di cifratura
                System.out.println("Inserisci la chiave di cifratura (numero):");
                int chiaveCifra = scanner.nextInt();
                
                //cifratura e stampa del risultato
                String risultatoCifra = cifratura(testoCifra, chiaveCifra);
                matrixEffect();
                System.out.print("\033[H\033[2J");
                System.out.flush();
                System.out.println("Testo cifrato: " + risultatoCifra);
                break;

            case 2:

                System.out.print("\033[H\033[2J");
                System.out.flush();

                //richiesta testo da decifrare
                System.out.println("Inserisci il testo da decifrare:");
                scanner.nextLine(); //consuma il newline rimasto
                String testoDecifra = scanner.nextLine();
                
                //richiesta chiave di decifratura
                System.out.println("Inserisci la chiave di decifratura (numero):");
                int chiaveDecifra = scanner.nextInt();
                
                //decifratura e stampa del risultato
                String risultatoDecifra = decifratura(testoDecifra, chiaveDecifra);
                matrixEffect();
                System.out.print("\033[H\033[2J");
                System.out.flush();
                System.out.println("Testo decifrato: " + risultatoDecifra);
                break;

            case 3:
                System.out.println("Uscita dal programma.");
                break;

            default:
                System.out.println("Scelta non valida.");
        }

        scanner.close();
        // --- RESETTA IL COLORE AL TERMINE DELL'ESECUZIONE ---
        System.out.print(ANSI_RESET);

    }

    public static void matrixEffect() {
        Random random = new Random();
        int numberOfLines = 100; // Numero di righe per l'effetto
        int charactersPerLine = 400; // Numero di caratteri per riga
        long lineDelayMillis = 15; // Ritardo tra la stampa di ogni riga

        System.out.println("Avvio del protocollo di simulazione...");

        for (int i = 0; i < numberOfLines; i++) {
            StringBuilder line = new StringBuilder();
            for (int j = 0; j < charactersPerLine; j++) {
                // Genera caratteri casuali stampabili (ASCII 33-126)
                char randomChar = (char) (random.nextInt(94) + 33);
                line.append(randomChar);
            }
            System.out.println(line.toString());

            try {
                TimeUnit.MILLISECONDS.sleep(lineDelayMillis);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Effetto Matrix interrotto.");
            }
        }
    }
}