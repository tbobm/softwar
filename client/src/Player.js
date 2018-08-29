import * as CT from './Data';
import { parseCommand, parseResponse } from './serverUtil';

export default class Player {
  constructor(socketManager, energy = 100, looking = 1) {
    this.energy = energy;
    this.looking = looking;
    this.socketManager = socketManager;
    this.nextCommand = [];
    this.enemyInSight = false;
    this.canCommunicate = true;
    this.commandFailed = false;
    this.lastCommand = '';
    this.alive = false;
    this.enemyInSight = false;
    this.sawProcess = [];
    this.possiblePath = [];
    this.currentLookingBounds = false;
    this.destination = [];
    this.createPath();
    console.log('Player is ready');
  }

  init() {
    this.socketManager.init();
    this.socketManager.sock.on('message', (msg) => {
      this.processMessage(parseResponse(msg.toString()));
    });
    this.identify(this.socketManager.id);
    return new Promise((resolve, reject) => {
      this.socketManager.sock.on('message', () => {
        resolve(true);
      });
      setTimeout(() => reject(new Error('Server is unreachable')), CT.TIMEOUT);
    });
  }

  evaluatePath(index) {
    this.destination = this.possiblePath[index];
  }

  createPath() {
    this.possiblePath = [
      [CT.FORWARD],
      [CT.FORWARD, CT.FORWARD, CT.LEFT],
      [CT.FORWARD, CT.FORWARD],
      [CT.FORWARD, CT.FORWARD, CT.RIGHT],
    ];
  }

  waitForResponse() {
    return new Promise((resolve) => {
      setInterval(() => {
        if (this.canCommunicate) {
          resolve(true);
        }
      }, 100);
    });
  }

  processMessage(message) {
    switch (parseResponse(this.lastCommand).status) {
      case CT.FORWARD:
      case CT.BACKWARD:
      case CT.LEFT:
      case CT.RIGHT:
        if (message.status === 'KO') {
          this.currentLookingBounds = true;
        }
        break;
      case CT.LOOKING:
        this.looking = message.data;
        break;
      case CT.WATCH:
        try {
          this.processWatch(message.data);
        } catch (e) {
          console.log(message.data, e);
        }
        break;
      case CT.SELFID:
        this.energy = message.data;
        break;
      default:
    }
    this.canCommunicate = true;
  }

  genericPreModifications(command, argument = null) {
    if (!this.canCommunicate) return false;
    const newCommand = parseCommand(command, argument);
    this.canCommunicate = false;
    this.commandFailed = false;
    this.lastCommand = newCommand;
    return true;
  }

  genericPostModifications() {
    this.canCommunicate = true;
  }

  identify(id) {
    if (!this.genericPreModifications(CT.IDENTIFY, id)) {
      return;
    }
    this.socketManager.send(this.lastCommand);
  }

  moveFunctions(direction) {
    if (!this.genericPreModifications(direction)) {
      return;
    }
    this.currentLookingBounds = false;
    this.socketManager.send(this.lastCommand);
  }

  rotationFunctions(direction) {
    if (!this.genericPreModifications(direction)) {
      return;
    }
    this.socketManager.send(this.lastCommand);
  }

  watch() {
    if (!this.genericPreModifications(CT.WATCH)) {
      return;
    }
    this.socketManager.send(this.lastCommand);
  }

  parseWatch(string) {
    return string.replace('[','').replace(']', '').split(',');
  }

  processWatch(data) {
    const array = this.parseWatch(data);
    for (let i = 0; i < array.length; i += 1) {
      switch (array[i]) {
        case 'empty':
          break;
        case '':
          break;
        case 'energy':
          this.evaluatePath(i);
          return;
        default:
          this.enemyInSight = true;
          return;
      }
    }
  }

  gather() {
    if (!this.genericPreModifications(CT.GATHER)) {
      return;
    }
    this.sockerManager.send(this.lastCommand, () => {
      this.canCommunicate = true;
    });
  }

  attack() {
    if (!this.genericPreModifications(CT.ATTACK)) {
      return;
    }
    this.sockerManager.send(this.lastCommand, () => {
      this.canCommunicate = true;
    });
  }

  getId() {
    if (!this.genericPreModifications(CT.SELFID)) {
      return;
    }
    this.sockerManager.send(this.lastCommand, (id) => {
      this.id = id;
      this.canCommunicate = true;
    });
  }

  getEnegy() {
    if (!this.genericPreModifications(CT.SELFSTATS)) {
      return;
    }
    this.socketManager.send(this.lastCommand, (energy) => {
      this.canCommunicate = true;
      this.energy = energy;
    });
  }

  analytic(id) {
    if (!this.genericPreModifications(CT.INSPECT, id)) {
      return;
    }
    this.socketManager.send(this.lastCommand, (energy) => {
      this.energy = energy;
      this.canCommunicate = true;
    });
  }

  pass() {
    if (!this.genericPreModifications(CT.NEXT)) {
      return;
    }
    this.socketManager.send(this.lastCommand, () => {
      this.canCommunicate = true;
    });
  }

  jump() {
    if (!this.genericPreModifications(CT.JUMP)) {
      return;
    }
    this.socketManager.send(this.lastCommand);
  }

  async play() {
    this.alive = true;
    while (this.alive) {
      await this.waitForResponse();
      if (this.enemyInSight) {
        this.attack();
      } else if (this.currentLookingBounds) {
        this.moveFunctions(CT.RIGHT);
      } else if (this.destination.length > 0) {
        this.moveFunctions(this.destination[0]);
        this.destination.shift();
      } else {
        this.watch();
      }
    }
  }
}
