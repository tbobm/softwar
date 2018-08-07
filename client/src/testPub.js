var zmq = require('zeromq')
    , sock = zmq.socket('pub');

sock.bindSync('tcp://127.0.0.1:3001');
console.log('Publisher bound to port 3000');

setInterval(function () {
    console.log('sending a multipart message envelope');
    sock.send(['#general', 'meow!']);
}, 500);