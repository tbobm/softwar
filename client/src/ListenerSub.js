import zmq from 'zeromq';
import { getRandomInt } from './serverUtil';

let instance = null;

export default class ListenerSub {
  constructor(address, port) {
    this.sock = zmq.socket('req');
    this.address = address;
    this.port = port;
    this.id = getRandomInt(1000);
    if (!instance) instance = this;
    return instance;
  }

  init() {
    this.sock.identity = Buffer.from(this.id.toString());
    console.log('Attempting connection to ', this.address, 'with port ', this.port);
    this.sock.connect(`tcp://${this.address}:${this.port}`);
  }

  send(message, callback) {
    this.sock.send(message, callback);
  }
}
