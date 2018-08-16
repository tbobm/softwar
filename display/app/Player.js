import Drawable from './Drawable';

export default class Player extends Drawable {
    constructor(playerStruct, tilesize, image) {
        super(tilesize, { x: playerStruct.x, y: playerStruct.y }, image);
        this.energy = playerStruct.energy;
        this.looking = playerStruct.looking;
        this.name = playerStruct.name;
    }
}