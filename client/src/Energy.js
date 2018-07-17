
import Drawable from './Drawable';

export default class Energy extends Drawable {
  constuctor(drawableKit, value) {
    super({ ...drawableKit });
    this.value = value;
  }
}
