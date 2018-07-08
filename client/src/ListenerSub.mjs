import zmq from 'zeromq';

let instance = null;
export default class ListenerSub {

	constructor(address, port) {
		this.sock = zmq.socket('req');
		this.sock.connect(`tcp://${address}:${port}`);
		if (!instance) {
			instance = this;
		}
		return instance;
	}

	send(message, callback) {
		this.sock.send(message, callback);
	}

}