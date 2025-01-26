const websocket = require('ws');
const http = require('http');

const wss = new websocket.Server({ port: 8080 });

server.on('connection', (ws) => {

    console.log('[!] Connection Established');

    ws.on('connection', (ws) => {
        console.log('[!] Connection Established');

            ws.on('[+] message', (message) => {
            console.log('[!] Incoming: %s', message);

                ws.send('[?] hi');
            });

        });
    });

