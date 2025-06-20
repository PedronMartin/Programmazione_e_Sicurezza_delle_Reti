//Client

//flag per non scrivere più volte 'sta scrivendo...'
var flag = false;

//richiesta del nome utente
var name= prompt("What's your name?");
while(name==""){
    name=prompt("You have to choose a name. \n What's your name?")
}

//Query DOM
var message = document.getElementById('message'),
   sender = document.getElementById('sender'),
   btn = document.getElementById('send'),
   output = document.getElementById('output'),
   feedback = document.getElementById('feedback');

//elemento per notificare che un utente sta scrivendo
var notifica = document.getElementById('notifica');

//inizializzo il campo sender con il nome scelto
sender.innerHTML=name;  
sender.value=name;

//invio richiesta di connessione al server
var webSocket = io.connect();

//ascolto l'evento di click sul bottone di invio
btn.addEventListener('click', function(){
   if (message.value!="" ){    
 	   webSocket.emit('messaggio', {
        message: message.value,
 	   sender: sender.value,
 	   });
 	   message.value = "";
   }
});

//gestione dell'evento 'aggiornamento' che arriva dal server (scrive il messaggio nella chat in data)
webSocket.on('aggiornamento', function(data){
    flag = false; //reset flag quando si riceve un nuovo messaggio
    feedback.innerHTML = '';
    notifica.innerHTML = '';
    output.innerHTML += '<p><strong>' + data.sender + ': </strong>' + data.message + '</p>';
});


//invia l'evento 'typing' quando l'utente preme un tasto di scrittura
message.addEventListener('keydown', function() {
    //questo include il nome dell'utente che sta scrivendo
    webSocket.emit('typing', { sender: name });
});

//gestione dell'evento 'typing' che arriva dal server, ossia che un altro utente sta scrivendo
webSocket.on('typing', function(data) {
    //visualizza il messaggio "sta scrivendo..." nel div 'notifica'
    //chi scrive non è lo stesso utente che sta scrivendo in quanto nel server viene usato broadcast.emit
    //la flag serve per non scrivere più volte lo stesso messaggio
    if(!flag){
        notifica.innerHTML += '<p><em>' + data.sender + ' sta scrivendo...</em></p>';
        flag = true;
    }
});

/*
Una volta che la chat è stata aperta sul browser, è richiesto il nome utente. Si noti poi l’uso che Javascript fa degli
identificatori dei tag HTML5. Alla riga 22/23 viene usata la funzione inner.HTML per modificare il contenuto della
pagina con il nome utente con il quale si è effettuata la registrazione. A questo punto alla riga 27 viene creato il
socket che deve connettersi con il server. Alla riga 30 viene aggiunto un listener all’evento “click” del bottone
che, quando verrà premuto, invierà al server il proprio nome utente e il contenuto del testo del messaggio, così da
poterlo inoltrare agli altri utenti. Importante è notare che viene passata la stringa “message” che il server usa per
riconoscere e gestire questo evento. Alla riga 41 viene gestito l’evento di I/O ‘UploadChat’ che arriva dal server
che rilancia un dato scritto da qualche altro client usando le istruzioni alle righe 13...; infatti si nota che
l’oggetto “data” contiene i campi “sender” e “message” che sono stati valorizzati da qualche altro client usando le
istruzioni citate. Infine, alla riga 50 viene gestito l’evento “typing” che notifica al server che un utente sta
scrivendo un messaggio. Il server inoltra questo evento a tutti gli altri client che lo visualizzano nella chat.
*/