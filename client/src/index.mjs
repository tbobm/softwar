import Listener from './ListenerSub.mjs';
import { ADDRESS, PORT } from './Data.mjs';
import Player from './Player.mjs';
const sub = new Listener(ADDRESS, PORT);
const player = new Player(0, sub);

/* This is where eveything begin*/

