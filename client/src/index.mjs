import Listener from './ListenerSub.mjs';
import { ADDRESS, PORT } from './Data.mjs';
import Player from './Player.mjs';

/* This is where eveything begin*/



const { argv } = process;

if (argv.length != 4) {
    console.log("Incorrect number of arguments");
     process.exit();          
}
const host = argv[2];
const port = argv[3];
console.log("Attempting connection to ", host, "with port ", port);
const sub = new Listener(host, port);
console.log("Init Player");
const player = new Player(0);
console.log("Init Done")