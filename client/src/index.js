import zmq from 'zeromq';
import Listener from './ListenerSub';
import Player from './Player';

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
  try {
    const response = await player.init();
    console.log('Init done:', response);
    console.log('Waiting the game with id ', player.socketManager.id);
    sock.connect(`tcp://${host}:${portSub}`);
    sock.subscribe('#event');
    console.log('Subscriber connected to port ', portSub);
    sock.on('message', (message) => {
      const data = JSON.parse(message.toString().replace('#event:', ''));
      if (data.notification_type === 1) {
        player.play();
      }
      if (data.notification_type === 2) {
        console.log('I won son');
        process.exit();
      }
    });
  } catch (error) {
    console.log('Unable to contact the server, AUTODESTRUCTION ACTIVATED', error);
    process.exit();
  }
};

main();
