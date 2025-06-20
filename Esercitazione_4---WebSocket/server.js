//Server

/*
Express è una libreria di Node.js che permettere di costruire molto facilmente applicazioni web.
La variabile var express=require(‘express’) crea una variabile che per funzionare necessita che la libreria Express sia installata.
Successivamente questa variabile viene utilizzata per creare il server web in ascolto sulla porta 443 (modificato da 4000).
*/
var express = require('express');

/*
Socket.IO è una libreria JavaScript utilizzata per implementare il protocollo WebSocket e racchiude numerose funzioni,
incluse il broadcasting a tutti i socket collegati, il salvataggio dei dati riguardanti ciascun utente e l’approccio asincrono di I/O.
Come per la creazione della variabile Express, var socket=require(“socket.io”) crea una variabile che per funzionare ha bisogno che la libreria socket.io sia installata.
*/
var socket = require('socket.io');

//chat setup
var app = express();

//in questo momento il server è in attesa delle connessioni HTTP sulla porta 443 (modificato da 4000)
var server = app.listen(443, function(){
   console.log('waiting for HTTP requests on port 443,');
});

// Static files
/*
Con questa funzione viene specificato a Nodejs che una volta ricevuta una connessione deve andare a
cercare nella cartella public il file html da fornire al client. In questo caso il file index.html (default) nella cartella public.
*/
app.use(express.static('public'));

// Socket setup & pass server
/*
Una volta che la connessione è stata ricevuta, qui viene effettuato l'upgrade ad una connessione
websocket e il server si mette in attesa degli eventi ai quali rispondere
*/
var io = socket(server);
io.on('connection', function(webSocket){

   console.log('made webSocket connection', webSocket.id);

   //ricezione di un messaggio da inoltrare ai client
   /*
   Messaggio con il tag “message”, che farà sì che il server inoltri il
   messaggio a tutti i client connessi mediante l’oggetto chiamato “sockets” che raggruppa tutti i socket creati.
   */
   webSocket.on('messaggio', function(data){
       io.sockets.emit('aggiornamento', data);
   });

   //gestione di un evento disconnessione
   webSocket.on('disconnect', () => {
       console.log('Un utente si è disconnesso');
   });

   // gestione dell'evento 'typing' per notificare che un utente sta scrivendo
   webSocket.on('typing', (data) => {

       //Socket.IO usa broadcast.emit per questo scopo, inoltrando l'evento a tutti gli altri client eccetto il mittente
       webSocket.broadcast.emit('typing', data);
   });
});