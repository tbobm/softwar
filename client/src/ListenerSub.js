import zmq from 'zeromq';
import { getRandomInt } from './serverUtil';

export default class ListenerSub {
  constructor(address, port) {
    this.sock = zmq.socket('req');
    this.address = address;
    this.port = port;
    this.id = `#${getRandomInt(999)}`;
  }

  init() {
    this.sock.identity = Buffer.from(this.id.toString());
    console.log('Attempting connection to ', this.address, 'with port ', this.port);
    this.sock.connect(`tcp://${this.address}:${this.port}`);
  }

  send(message) {
    this.sock.send(message);
  }
}
