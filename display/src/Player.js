import Drawable from './Drawable';
import ProgressBar from 'progressbar.js'

export default class Player extends Drawable {
    constructor(playerStruct, tilesize, image, index, context) {
        super(tilesize, { x: playerStruct.x, y: playerStruct.y }, image);
        this.status.energy = playerStruct.energy;
        this.index = index;
        this.dataZone = document.getElementById(`p${index}`);
        this.status.looking = playerStruct.looking;
        this.status.stun_duration = playerStruct.stun_duration;
        this.name = playerStruct.name;
        this.isStunned = false;
        this.context = context;
        this.fillStyle = "#66CCFF";
        this.images = [];
        this.initImageFolder();
    }

    render() {
        if (this.status.energy > 0) {
          this.context.drawImage(this.images[this.status.looking] , this.status.x * this.size, this.status.y * this.size, this.size, this.size);
        }
    }

    updatePlayerStatus(newStatus) {
        if (this.status.energy <= 0) {
            return;
        }
        this.status = {...this.status, ...newStatus};
    };

    initImageFolder() {
        const path = `../app/images/perso${this.index}`;
        for(let i = 0; i < 4; i++) {
            let image = new Image();
            image.src = `../app/images/perso${this.index}/${i}.png`;
            this.images.push(image);
        }
    }

    updateDisplayedData() {
        this.dataZone.innerHTML = `${this.name}
        <br> Energy: ${this.status.energy}
        <br> Position x: ${this.status.x} y: ${this.status.y}
        <br> Looking: ${this.status.looking}
        <br> Stun: ${this.status.stun_duration}`
    }
}