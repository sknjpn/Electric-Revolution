machine = {
	name = "アイテムセンサー",
	size = {x = 1, y = 1},
	group = "入出力部品"
}

function init()
    setNode(0.25, 0.75, 0.0, 1.0, 1, true)
    setNode(0.75, 0.75, 0.0, 1.0, 2, true)
end

function updateSystem()

    if isItemPos(0, -1) then
        setNodeState(0, 1)
        setNodeState(1, 2)
    else
        setNodeState(0, 2)
        setNodeState(1, 1)
    end
end

function draw()
    drawTexture("image.png")

    if isItemPos(0, -1) then
        drawTexture("light.png")
    end
end