import MainLoop from 'mainloop.js';
import * as PIXI from 'pixi.js';

import 'styles/index.scss';
import data from './Data';
import { createGrid } from './mapHandler';
import Player from './Player';

const size = 500;
const tileSize = size / data.GameInfo.map_size;
const debug = true;
const app = new PIXI.Application({ width: size, height: size });

const initPlayerFromData = ({ GameInfo }, tileSize) => {
  const { players } = GameInfo;
  let playersClass = {};
  players.map((player) => {
    playersClass[player.name] = new Player(player, tileSize, "");
  })
  return playersClass;
};

const update = () => {
  if (debug)
    document.getElementById("fps").innerHTML = `fps: ${MainLoop.getFPS()}`;
}

const initGame = (app) => {
  createGrid(data, app, tileSize);
  const players = initPlayerFromData(data);
  console.log(players);
  document.body.appendChild(app.view);
  MainLoop.setUpdate(update).start();
}

initGame(app);