import * as CT from './Data';
import {
  parseCommand,
  parseResponse,
  wait, rollDice,
  getRandomInt,
  parseWatch,
} from './serverUtil';

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
    this.sawProcess = [];
    this.currentLookingBounds = false;
    this.destination = [];
    this.needToGather = false;
    this.possiblePath = CT.POSSIBLE_PATH;
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
    this.needToGather = true;
    this.destination = this.possiblePath[index];
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
    console.log('command replied to ', this.lastCommand, message);
    if (message.data === 'not enough action points') { 
      this.canCommunicate = true;
      return;
    }
    if (message.data === 'process dead') {
      console.log("I'm dead sir");
      process.exit();
    }
    switch (parseResponse(this.lastCommand).status) {
      case CT.LEFT:
        if (message.status === 'KO') {
          this.moveFunctions(CT.RIGHT);
        }
        break;
      case CT.RIGHT:
        if (message.status === 'KO') {
          this.moveFunctions(CT.LEFT);
        }
        break;
      case CT.BACKWARD:
      case CT.FORWARD:
        if (message.status === 'KO') {
          this.currentLookingBounds = true;
        }
        if (message.data === 'out of map') {
          this.needToTurnBack = 2;
        }
        break;
      case CT.LOOKING:
        this.looking = message.data;
        break;
      case CT.WATCH:
        this.processWatch(message.data);
        break;
      case CT.SELFID:
        this.energy = message.data;
        break;
      case CT.ATTACK:
        if (message.data === '0 player(s) have been attacked') {
          this.enemyInSight = false;
        }
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

  looking() {
    if (!this.genericPreModifications(CT.LOOKING)) {
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



  processWatch(data) {
    this.enemyInSight = false;
    const array = parseWatch(data);
    for (let i = 0; i < array.length; i += 1) {
      console.log('seen ', array[i]);
      switch (array[i]) {
        case 'empty':
          break;
        case 'x':
          break;
        case 'energy':
          this.evaluatePath(i);
          return;
        default:
          this.sawProcess[array[i]] = true;
          this.enemyInSight = true;
          return;
      }
    }
  }

  gather() {
    if (!this.genericPreModifications(CT.GATHER)) {
      return;
    }
    this.socketManager.send(this.lastCommand, () => {
      this.canCommunicate = true;
    });
  }

  attack() {
    if (!this.genericPreModifications(CT.ATTACK)) {
      return;
    }
    this.socketManager.send(this.lastCommand, () => {
      this.canCommunicate = true;
    });
  }

  getId() {
    if (!this.genericPreModifications(CT.SELFID)) {
      return;
    }
    this.socketManager.send(this.lastCommand, (id) => {
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

  async generateRandomBehaviour() {
    if (rollDice(80)) {
      this.moveFunctions(CT.randomMoves[getRandomInt(CT.randomMoves.length)]);
      await this.waitForResponse();
      this.watch();
    } else if (rollDice(10)) {
      this.jump();
    } else if (rollDice(30)) {
      this.attack();
    } else {
      this.watch();
    }
  }

  async play() {
    this.alive = true;
    while (this.alive) {
      await this.waitForResponse();
      if (this.needToTurnBack > 0) {
        this.rotationFunctions(CT.PIVLEFT);
        this.needToTurnBack -= 1;
      } else if (this.destination.length > 0) {
        this.moveFunctions(this.destination[0]);
        this.destination.shift();
      } else if (this.needToGather && this.destination.length === 0) {
        this.gather();
        this.needToGather = false;
      } else if (this.enemyInSight) {
        this.attack();
      } else if (this.currentLookingBounds) {
        if (rollDice(80)) {
          this.rotationFunctions(CT.PIVRIGHT);
        } else {
          this.rotationFunctions(CT.PIVLEFT);
        }
        await this.waitForResponse();
        this.moveFunctions(CT.FORWARD);
      } else {
        this.generateRandomBehaviour();
      }
    }
  }
}
