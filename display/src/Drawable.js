
export default class Drawable {
  constructor(size, position, image) {
    this.size = size;
    this.status = {};
    this.status.x = position.x;
    this.status.y = position.y;
    this.image = image;
  }
}
