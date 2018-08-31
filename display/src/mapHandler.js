export const createGrid = (data, app) => {
    for (var j = 0; j < data.map_size; j++) {
        for (var i = 0; i < data.map_size; i++) {
            var rectangle = new PIXI.Graphics()
            rectangle.lineStyle(1, 0xFF3300);
            rectangle.drawRect(i * data.tileSize, j * data.tileSize, data.tileSize, data.tileSize);
            app.stage.addChild(rectangle);
        };
    };
}
