function init()
    setNode(0.5, 1.0, 2, true)
end

function updateSystem()

    if isItemPos(0, -1) then
        setNodeState(0, 1)
    else
        setNodeState(0, 2)
    end
end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    if getNodeState(0) == 1 then
        drawTexture("node-hi.png")
    else
        drawTexture("node-low.png")
    end

    if isItemPos(0, -1) then
        drawTexture("light.png")
    end
end