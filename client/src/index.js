import zmq from 'zeromq';
import Listener from './ListenerSub';
import Player from './Player';
import { Wrapper } from './serverUtil';

require('babel-core/register');
/* This is where eveything begin */

const main = async () => {
  const { argv } = process;
  if (argv.length !== 5) {
    console.log('Incorrect number of arguments');
    process.exit();
  }
  const host = argv[2];
  const port = argv[3];
  const portSub = argv[4];
  const sub = new Listener(host, port);
  const sock = zmq.socket('sub');

  console.log('Init Player');
  const player = new Player(sub);
  // const gameStarted = Wrapper(() => playGame(player));
  try {
    const response = await player.init();
    console.log('Init done:', response);
    console.log('Waiting the game with id ', player.socketManager.id);
    sock.connect(`tcp://127.0.0.1:${portSub}`);
    sock.subscribe('#all');
    console.log('Subscriber connected to port ', portSub);
    let play = 0;
    sock.on('message', (message) => {
      play += 1;
      if (play === 1) {
        player.play();
        console.log(message.toString());
      }
    });
  } catch (error) {
    console.log('Unable to contact the server, AUTODESTRUCTION ACTIVATED', error);
    process.exit();
  }
};

main();
