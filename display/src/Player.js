import Drawable from './Drawable';

export default class Player extends Drawable {
    constructor(playerStruct, tilesize, image, index) {
        super(tilesize, { x: playerStruct.x, y: playerStruct.y }, image);
        this.status.energy = playerStruct.energy;
        this.index = index;
        this.dataZone = document.getElementById(`p${index}`);
        this.status.looking = playerStruct.looking;
        this.status.stun_duration = playerStruct.stun_duration;
        this.name = playerStruct.name;
        this.isStunned = false;
        let rectangle = new PIXI.Graphics();
        rectangle.beginFill(0x66CCFF);
        rectangle.drawRect(this.status.x * this.size, this.status.y * this.size, this.size, this.size);
        rectangle.endFill();
        this.sprite = rectangle;
        
    }

    updatePlayerStatus(newStatus, app) {
        if (this.status.energy <= 0) {
            this.sprite.clear();
            return;
        }
        this.status = {...this.status, ...newStatus};
        this.sprite.x = this.status.x * this.size;
        this.sprite.y = this.status.y * this.size;
        //this.updateProcessColor();
        app.stage.addChild(this.sprite);
    };

    updateProcessColor() {
        if (this.status.stun_duration === 0 && this.isStunned) {
            this.sprite.clear();
            this.isStunned = false;
            this.sprite.beginFill(0x66CCFF);
        } else if (this.status.stun_duration > 0) {
            this.sprite.clear();
            this.isStunned = true;
            this.sprite.beginFill(0x660066);
        } else {
            return ;
        }
        this.sprite.drawRect(this.status.x * this.size, this.status.y * this.size, this.size, this.size);
        this.sprite.endFill();
    }

    updateDisplayedData() {
        this.dataZone.innerHTML = `${this.name}
        <br> Energy: ${this.status.energy}
        <br> Position x: ${this.status.x} y: ${this.status.y}
        <br> Looking: ${this.status.looking}`
    }
}