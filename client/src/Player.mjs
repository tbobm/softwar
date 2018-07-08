import * as CT from './Data.mjs';
import { parseCommand, parseResponse } from './serverUtil.mjs';

export default class Player {

	constuctor(id, socketManager, energy = 100, looking = 1) {
		this.id = id;
		this.energy = energy;
		this.looking = looking;
		this.socketManager = socketManager;
		this.nextCommands = [];
		this.enemyInSight = false;
		this.canCommunicate = true;
		this.commandFailed = false;
		this.nextCommand = "";
	}

	genericPreModifications(command) {
		const command = parseCommand(command);
		this.canCommunicate = false;
		this.commandFailed = false;
		this.nextCommand = command;
	}

	genericPostModifications() {

	}

	moveFunctions(direction) {
		// Process position with adequate function helper
		//const newPosition = this.position + direction
		genericPreModifications(CT.MOVE);
		this.socketManager.send(this.nextCommand, (status) => {
			this.canCommunicate = true;
			this.position = direction;
			console.log("Position for player", this.id, "is", direction);
		});
	}

	rotationFunctions(direction) {
		// Process position with adequate function helper
		//const newPosition = this.position + direction
		genericPreModifications(CT.LOOKING);
		this.socketManager.send(this.nextCommand, () => {
			this.canCommunicate = true;
			this.looking = direction;
			console.log("Orientation for player", this.id, "is", direction);
		});
	}

	watch() {
		genericPreModifications(CT.WATCH);
		this.socketManager.send(this.nextCommand, (response) => {
			const data = parseResponse(response)[1];
			if (!data) return;
			this.canCommunicate = true;
			data.map((tile, index) => {
				if (tile === "empty") return;
				//process tiles
			});
		});
	}

	gather() {
		genericPreModifications(CT.GATHER);
		this.sockerManager.send(this.nextCommand, () => {
			this.canCommunicate = true;
		});
	}

	attack() {
		genericPreModifications(CT.ATTACK);
		this.sockerManager.send(this.nextCommand, () => {
			this.canCommunicate = true;
		});
	}

	getId() {
		genericPreModifications(CT.SELFID);
		this.sockerManager.send(this.nextCommand, (id) => {
			this.id = id;
			this.canCommunicate = true;
		});
	}

	getEnegy() {
		genericPreModifications(CT.SELFSTATS);
		this.socketManager.send(this.nextCommand, (energy) => {
			this.canCommunicate = true;
			this.energy = energy;
		});
	}

	analytic(id) {
		genericPreModifications(CT.INSPECT);
		this.socketManager.send(this.nextCommand, (energy) => {
			this.energy = energy;
			this.canCommunicate = true;
		});
	}

	pass() {
		genericPreModifications(CT.NEXT);
		this.socketManager.send(this.nextCommand, () => {
			this.canCommunicate = true;
		});
	}

	jump() {
		genericPreModifications(CT.JUMP);
		this.socketManager.send(this.nextCommand, (status) => {
			if (status == "ko") return;
			this.canCommunicate = true;
			this.position = newPosition;
			console.log("Position for player", this.id, "is", newPosition);
		});
	}
}