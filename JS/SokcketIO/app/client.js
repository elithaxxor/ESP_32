const socket = new WebSocket('ws://localhost:3000');
socket.onmessage = ({ data })=> {
    console.log('[!] Connection Established');
   // socket.send('[?] bye ');
};

document.querySelector('button').onclick = () => {
    socket.send('[?] ');
}

