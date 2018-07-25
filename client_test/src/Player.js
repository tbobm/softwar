import * as CT from './Data';
import { parseCommand, parseResponse } from './serverUtil';

export default class Player {
  constructor(id, socketManager = null, energy = 100, looking = 1) {
    console.log('Player is ready');
    this.id = id;
    this.energy = energy;
    this.looking = looking;
    this.socketManager = socketManager;
    this.nextCommands = [];
    this.enemyInSight = false;
    this.canCommunicate = true;
    this.commandFailed = false;
    this.nextCommand = '';
  }

  genericPreModifications(command, argument = null) {
    const newCommand = parseCommand(command, argument);
    this.canCommunicate = false;
    this.commandFailed = false;
    this.nextCommand = newCommand;
  }

  genericPostModifications() {
    this.canCommunicate = true;
  }

  moveFunctions(direction) {
    // Process position with adequate function helper
    // const newPosition = this.position + direction
    this.genericPreModifications(CT.MOVE);
    this.socketManager.send(this.nextCommand, () => {
      this.canCommunicate = true;
      this.position = direction;
      console.log('Position for player', this.id, 'is', direction);
    });
  }

  rotationFunctions(direction) {
    // Process position with adequate function helper
    // const newPosition = this.position + direction
    this.genericPreModifications(CT.LOOKING);
    this.socketManager.send(this.nextCommand, () => {
      this.canCommunicate = true;
      this.looking = direction;
      console.log('Orientation for player', this.id, 'is', direction);
    });
  }

  watch() {
    this.genericPreModifications(CT.WATCH);
    this.socketManager.send(this.nextCommand, (response) => {
      const data = parseResponse(response)[1];
      if (!data) return;
      this.canCommunicate = true;
      data.map((tile) => {
        if (tile !== 'empty') {}
        // process tiles
      });
    });
  }

  gather() {
    this.genericPreModifications(CT.GATHER);
    this.sockerManager.send(this.nextCommand, () => {
      this.canCommunicate = true;
    });
  }

  attack() {
    this.genericPreModifications(CT.ATTACK);
    this.sockerManager.send(this.nextCommand, () => {
      this.canCommunicate = true;
    });
  }

  getId() {
    this.genericPreModifications(CT.SELFID);
    this.sockerManager.send(this.nextCommand, (id) => {
      this.id = id;
      this.canCommunicate = true;
    });
  }

  getEnegy() {
    this.genericPreModifications(CT.SELFSTATS);
    this.socketManager.send(this.nextCommand, (energy) => {
      this.canCommunicate = true;
      this.energy = energy;
    });
  }

  analytic(id) {
    this.genericPreModifications(CT.INSPECT, id);
    this.socketManager.send(this.nextCommand, (energy) => {
      this.energy = energy;
      this.canCommunicate = true;
    });
  }

  pass() {
    this.genericPreModifications(CT.NEXT);
    this.socketManager.send(this.nextCommand, () => {
      this.canCommunicate = true;
    });
  }

  jump() {
    this.genericPreModifications(CT.JUMP);
    this.socketManager.send(this.nextCommand, (status) => {
      if (status === 'ok') {
        this.canCommunicate = true;
      }
      // this.position = newPosition;
      // console.log('Position for player', this.id, 'is', newPosition);
    });
  }
}
