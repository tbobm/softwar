import zmq from 'zeromq';
import { TIMEOUT } from './Data';

let instance = null;
export default class ListenerSub {
  constructor(address, port) {
    this.sock = zmq.socket('req');
    this.address = address;
    this.port = port;
    this.id = Math.random(1000);
    if (!instance) instance = this;
    return instance;
  }

  init() {
    this.sock.identity = this.id;
    this.sock.connect(`tcp://${this.address}:${this.port}`);
    console.log('Attempting connection to ', this.address, 'with port ', this.port);
    this.sock.send('Hello');
    return new Promise((resolve, reject) => {
      setTimeout(() => reject(new Error('Server is unreachable')), TIMEOUT);
      this.sock.on('message', () => {
        resolve(true);
      });
    });
  }

  send(message, callback) {
    this.sock.send(message, callback);
  }
}
