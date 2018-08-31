export const createGrid = (data, ctx) => {
    for (let j = 0; j < data.map_size; j++) {
        for (let i = 0; i < data.map_size; i++) {
            ctx.beginPath();
            ctx.lineWidth = "1";
            ctx.strokeStyle = "#0xFF3300";
            ctx.rect(i * data.tileSize, j * data.tileSize, data.tileSize, data.tileSize);
            ctx.stroke();
        };
    };
}
