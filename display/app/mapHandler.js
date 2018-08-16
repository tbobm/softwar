export const createGrid = (data, app, tileSize) => {
    for (var j = 0; j < data.GameInfo.map_size; j++) {
        for (var i = 0; i < data.GameInfo.map_size; i++) {
            var rectangle = new PIXI.Graphics()
            rectangle.lineStyle(1, 0xFF3300);
            rectangle.drawRect(i * tileSize, j * tileSize, tileSize, tileSize);
            app.stage.addChild(rectangle);
        };
    };
}
