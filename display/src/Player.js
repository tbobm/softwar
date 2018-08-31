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
        this.fillStyle = "#0x66CCFF";
    }

    render() {
        this.context.beginPath();
        this.context.fillStyle = this.fillStyle;
        this.context.fillRect(this.status.x * this.size, this.status.y * this.size, this.size, this.size);
        this.context.stroke();
    }

    updatePlayerStatus(newStatus) {
        if (this.status.energy <= 0) {
            // delete canvas
            return;
        }
        this.status = {...this.status, ...newStatus};
        this.updateProcessColor();
    };

    updateProcessColor() {
        this.fillStyle = "#0x66CCFF";
        if (this.status.stun_duration === 0 && this.isStunned) {
            this.isStunned = false;
            this.fillStyle = "0x66CCFF";
        } else if (this.status.stun_duration > 0) {
            this.isStunned = true;
            this.fillStyle = "#F6546A";
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