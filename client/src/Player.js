import Drawable from './Drawable';
import { UP, DOWN, LEFT, RIGHT } from './Constantes';

export default class Player extends Drawable {
	constuctor(drawableKit, id, energy, looking) {
		super({...drawableKit});
		this.id = id;
		this.energy = energy;
		this.looking = looking;
	}

	moveHorizontaly(direction) {
		this.position.x += 1 * direction;
		this.looking = direction == 1 ? RIGHT || lEFT;
	}

	moveVerticaly(direction) {
		this.position.y += 1 * direction;
		this.looking = direction == 1 ? DOWN || UP; 
	}

	changeOrientation(looking) {
		this.looking = looking;
	}
	
}