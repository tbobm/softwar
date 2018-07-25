var zmq = require('zeromq')
  , sock = zmq.socket('rep');

sock.bindSync('tcp://127.0.0.1:3000');
console.log('Producer bound to port 3000');

sock.on('message', (msg) => {
    console.log(msg.toString());
    sock.send("World");
})