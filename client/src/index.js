
import Listener from './ListenerSub';
import Player from './Player';
import zmq from 'zeromq';

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
  console.log('Init Player');
  const player = new Player(0, sub);
  try {
    const response = await player.socketManager.init();
    console.log('Init done:', response);
    console.log('Waiting the game with id ', player.socketManager.id);
    sock.connect(`tcp://127.0.0.1:${portSub}`);
    sock.subscribe('#general');
    console.log('Subscriber connected to port ', portSub);
    sock.on('message', function (message) {
      console.log(message.replace("#general: ", ''));
    });

  } catch (error) {
    console.log('Unable to contact the server, AUTODESTRUCTION ACTIVATED');
    process.exit();
  }
};



main();
