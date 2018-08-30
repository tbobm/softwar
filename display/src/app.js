// ----------------------------------------------------------------------------
// Everything below is just to show you how it works. You can delete all of it.
// ----------------------------------------------------------------------------
import MainLoop from 'mainloop.js';
import * as PIXI from 'pixi.js';
import { createGrid } from './mapHandler';
import Player from './Player';
import zmq from 'zeromq';

const { argv } = process;
if (argv.length !== 4) {
    console.log('Incorrect number of arguments');
    console.log(argv);
    //process.exit();
}

const host = 'localhost'
const portSub = '4243';
const sock = zmq.socket('sub');
const size = 500;
const app = new PIXI.Application({ width: size, height: size });
let gameInfo = {};
let players = {};
let game_started = false;

sock.connect(`tcp://${host}:${portSub}`);
sock.subscribe('#');
console.log('Subscriber connected to port ', portSub);
sock.on('message', (message) => {
    const string_msg = message.toString();
    if (string_msg.includes('#event')) {
      return;
    };
    const data = JSON.parse(string_msg.replace('#all:', ''));
    data.data.tileSize = size / data.data.map_size;
    if (data.notification_type === 0) {
      gameInfo = data.data;
    } else if (data.notification_type == 2) {
      MainLoop.stop();
    }
    if (!game_started) {
      game_started = true;
      initGame(app);
    }
});

const update = () => {
  document.getElementById("fps").innerHTML = `fps: ${MainLoop.getFPS()}`;
    updatePlayers();
    drawEnergyCell(gameInfo);
}

const updatePlayers = () => {
  const { list_players } = gameInfo;
  list_players.map((player) => {
    players[player.name].updatePlayerStatus(player, app);
    players[player.name].updateDisplayedData();
  });
}
const drawEnergyCell = ({list_energy_cells}) => {
  list_energy_cells.map((cell) => {
      let displayedEnegy = new PIXI.Graphics();
      displayedEnegy.beginFill(0xF6546A);
      displayedEnegy.drawRect(cell.x * gameInfo.tileSize, cell.y * gameInfo.tileSize, gameInfo.tileSize/2, gameInfo.tileSize/2);
      displayedEnegy.endFill();
      app.stage.addChild(displayedEnegy);
  });
} 
const initPlayerFromData = () => {
  const { list_players } = gameInfo;
  let playersClass = {};
  list_players.map((player, index) => {
    playersClass[player.name] = new Player(player, gameInfo.tileSize, "", index);
    //playersClass[player.name].sprite = rectangle;
  })
  return playersClass;
};

const initGame = () => {
  createGrid(gameInfo, app);
  players = initPlayerFromData();
  document.getElementById("game").appendChild(app.view);
  MainLoop.setMaxAllowedFPS(30);
  MainLoop.setUpdate(update).start();
}