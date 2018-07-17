
import Listener from './ListenerSub';
import Player from './Player';

require('babel-core/register');

/* This is where eveything begin */


const main = async () => {
  const { argv } = process;
  if (argv.length !== 4) {
    console.log('Incorrect number of arguments');
    process.exit();
  }
  const host = argv[2];
  const port = argv[3];
  const sub = new Listener(host, port);
  console.log('Init Player');
  const player = new Player(0, sub);
  try {
    const response = await player.socketManager.init();
    console.log('Init done:', response);
    console.log('Waiting the game');
  } catch (error) {
    console.log('Unable to contact the server, AUTODESTRUCTION ACTIVATED');
    process.exit();
  }
};

main();
