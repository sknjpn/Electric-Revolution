machine = {
	name = "電池",
	size = {x = 2, y = 1},
	group = "入出力部品"
}

function init()
    setNode(0.25, 0.5, -1.0, 0.0, 1, true)
    setNode(1.75, 0.5,  1.0, 0.0, 2, true)
end

function updateSystem()

end

function updateConnects()

end

function draw()
    drawTexture("image.png")
end