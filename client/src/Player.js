import Drawable from './Drawable';
import CT from './Constantes';
import { parseCommand } from './serverUtil';

export default class Player extends Drawable {
	
	constuctor(drawableKit, id, energy, looking, socketManager) {
		super({...drawableKit});
		this.id = id;
		this.energy = energy;
		this.looking = looking;
		this.socketManager = socketManager;
		this.nextCommands = [];
		this.enemyInSight = false;
	}

	moveFunctions(direction) {
		// Process position with adequate function helper
		//const newPosition = this.position + direction
		const command = parseCommand(CT.WATCH);
		this.socketManager.send(direction, () => {
			this.position = direction;
			console.log("Position for player", this.id, "is", direction);
		});
	}

	rotationFunctions(direction) {
		// Process position with adequate function helper
		//const newPosition = this.position + direction
		const command = parseCommand(CT.WATCH);
		this.socketManager.send(direction, () => {
			this.looking = direction;
			console.log("Orientation for player", this.id, "is", direction);
		});
	}

	watch() {
		const command = parseCommand(CT.WATCH);
		this.socketManager.send(command, (tiles) => {
			tiles.map((tile, index) => {
				if (tile === "empty") return;
				//process tiles
			});
		});
	}

	gather() {
		const command = parseCommand(CT.GATHER);
		this.sockerManager.send(command, () => {});
	}

	attack() {
		const command = parseCommand(CT.ATTACK);
		this.sockerManager.send(command, () => {});
	}

	getId() {
		const command = parseCommand(CT.SELFID);
		this.sockerManager.send(command, (id) => {
			this.id = id;
		});
	}

	getEnegy() {
		const command = parseCommand(CT.SELFSTATS);
		this.socketManager.send(command, (energy) => {
			this.energy = energy;
		});
	}

	analytic(id) {
		const command = parseCommand(CT.INSPECT);
		this.socketManager.send(command, (energy) => {
			this.energy = energy;
		});
	}

	pass() {
		const command = parseCommand(CT.NEXT);
		this.socketManager.send(command, () => {});
	}

	jump() {
		const command = parseCommand(command CT.JUMP);
		this.socketManager.send(command, (newPosition) => {
			this.position = newPosition;
			console.log("Position for player", this.id, "is", newPosition);
		});
	}
}