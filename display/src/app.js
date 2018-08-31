// ----------------------------------------------------------------------------
// Everything below is just to show you how it works. You can delete all of it.
// ----------------------------------------------------------------------------
import MainLoop from 'mainloop.js';
import { createGrid } from './mapHandler';
import Player from './Player';
import zmq from 'zeromq';
const prompt = require('electron-prompt');

const custom_prompt = (title, default_value) => new Promise((resolve) => {
  prompt({
      title: 'Prompt example',
      label: title,
      value: default_value,
      inputAttrs: {
          type: 'text'
      },
      type: 'input',
  })
  .then((r) => {
      resolve(r);
  }).catch(console.error);
});

const getCanvas = (id) => {
  const canvas = document.getElementById(id);
  canvas.width = size;
  canvas.height = size;
  return canvas;
}

const sock = zmq.socket('sub');
const size = 500;
let gameInfo = {};
let players = {};
let game_started = false;
const playerCanevas = getCanvas('players').getContext('2d');
const mapCanvas = getCanvas('map').getContext('2d');
const cellCanvas = getCanvas('cells').getContext('2d');


const get_data = async () => {
  const host = await custom_prompt('host', 'localhost');
  const portSub = await custom_prompt('port', 4243);
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
}


const update = () => {
  drawCells();
  drawPlayer();
}

const drawPlayer = () => {
    playerCanevas.clearRect(0,0,size,size);
    updatePlayers();
}

const drawCells = () => {
    cellCanvas.clearRect(0,0,size,size);
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
    let image = new Image();
    image.src = `../app/images/coin.png`;
    cellCanvas.drawImage(image, cell.x * gameInfo.tileSize, cell.y * gameInfo.tileSize, gameInfo.tileSize, gameInfo.tileSize);
  });
} 

const initPlayerFromData = () => {
  const { list_players } = gameInfo;
  let playersClass = {};
  list_players.map((player, index) => {
    playersClass[player.name] = new Player(player, gameInfo.tileSize, "", index, playerCanevas);
    
  })
  return playersClass;
};

const initGame = () => {
  createGrid(gameInfo, mapCanvas);
  players = initPlayerFromData();
  MainLoop.setMaxAllowedFPS(30);
  MainLoop.setUpdate(update).start();
}
get_data();