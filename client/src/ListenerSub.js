import zmq from 'zeromq';

let instance = null

export default class ListenerSub {
	
	constructor(address, port) {
		this.sock = zmq.socket('req');
		this.sock.connect(`tcp://${address}:${port}`);
		if (!instance) {
  			instance = this;
		}
		return instance;
	}

	send(message) {
		this.sock.send(message, (rep) => {
			console.log(rep);
		})
	}

}