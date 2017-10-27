machine = {
	name = "配電盤",
	size = {x = 1, y = 1},
	group = "入出力部品"
}

function init()
    setNode(0.25, 0.50, -1.0,  0.0)
    setNode(0.50, 0.25,  0.0, -1.0)
    setNode(0.75, 0.50,  1.0,  0.0)
    setNode(0.50, 0.75,  0.0,  1.0)
end

function draw()
    drawTexture("image.png")
end