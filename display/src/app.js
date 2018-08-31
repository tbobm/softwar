// ----------------------------------------------------------------------------
// Everything below is just to show you how it works. You can delete all of it.
// ----------------------------------------------------------------------------
import MainLoop from 'mainloop.js';
import { createGrid } from './mapHandler';
import Player from './Player';
import zmq from 'zeromq';

const { argv } = process;
if (argv.length !== 4) {
    console.log('Incorrect number of arguments');
    console.log(argv);
    //process.exit();
}
const getCanvas = (id) => {
  const canvas = document.getElementById(id);
  canvas.width = size;
  canvas.height = size;
  return canvas;
}
const host = 'localhost'
const portSub = '4243';
const sock = zmq.socket('sub');
const size = 500;
let gameInfo = {};
let players = {};
let game_started = false;
const playerCanevas = getCanvas('players');
const mapCanvas = getCanvas('map').getContext('2d');
const cellCanvas = getCanvas('cells').getContext('2d');

sock.connect(`tcp://${host}:${portSub}`);
sock.subscribe('#');
console.log('Subscriber connected to port ', portSub);

sock.on('message', (message) => {
    const string_msg = message.toString();
    if (string_msg.includes('#event')) {
      const msg = JSON.parse(string_msg.replace('#event:', ''));
      if (msg.notification_type == 2 || msg.notification_type == 4) {
        MainLoop.stop()
        // display winner here
      }
      return;
    };
    const data = JSON.parse(string_msg.replace('#all:', ''));
    data.data.tileSize = Math.floor(size / data.data.map_size);
    if (data.notification_type === 0) {
      gameInfo = data.data;
    }
    if (!game_started) {
      game_started = true;
      initGame();
    }
});

const update = () => {
  document.getElementById("fps").innerHTML = `fps: ${MainLoop.getFPS()}`;
    playerCanevas.getContext('2d').clearRect(0,0,size,size);
    cellCanvas.clearRect(0,0,size,size);
    updatePlayers();
    cellCanvas.beginPath();
    drawEnergyCell(gameInfo);
    cellCanvas.stroke();
}

const updatePlayers = () => {
  const { list_players } = gameInfo;
  list_players.map((player) => {
    players[player.name].updatePlayerStatus(player);
    players[player.name].updateDisplayedData();
    players[player.name].render();
  });
}
const drawEnergyCell = ({list_energy_cells}) => {
  list_energy_cells.map((cell) => {
    cellCanvas.fillStyle = "0xF6546A";
    cellCanvas.fillRect(cell.x * gameInfo.tileSize, cell.y * gameInfo.tileSize, gameInfo.tileSize/2, gameInfo.tileSize/2);
  });
} 
const initPlayerFromData = () => {
  const { list_players } = gameInfo;
  let playersClass = {};
  list_players.map((player, index) => {
    playersClass[player.name] = new Player(player, gameInfo.tileSize, "", index, playerCanevas.getContext('2d'));
    
  })
  return playersClass;
};

const initGame = () => {
  createGrid(gameInfo, mapCanvas);
  players = initPlayerFromData();
  MainLoop.setMaxAllowedFPS(30);
  MainLoop.setUpdate(update).start();
}