/**
 * Application entry point
 */

// Load application styles
import 'styles/index.scss';
import MainLoop from 'mainloop.js';
import * as Pixi from 'pixi.js';

const update = () => {
  rectangle.x += 1;
  rectangle.y += 1;
}
MainLoop.setUpdate(update).start();
document.getElementById("fps").innerHTML = MainLoop.getFPS(); 

const app = new PIXI.Application({width: 500, height: 500});

// The application will create a canvas element for you that you
// can then insert into the DOM
document.body.appendChild(app.view);

// load the texture we need
let rectangle = new PIXI.Graphics();
rectangle.beginFill(0x66CCFF);
rectangle.drawRect(0, 0, 64, 64);
rectangle.endFill();
app.stage.addChild(rectangle);
