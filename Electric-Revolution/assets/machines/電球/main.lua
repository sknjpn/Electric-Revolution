function init()
    setNode(0.0, 1.5, -2.0, 0.0)
    setNode(4.0, 1.5,  2.0, 0.0)
end

function updateSystem()

end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    if getNodeState(0) == 1 then
        drawTexture("node0-hi.png")
    elseif getNodeState(0) == 2 then
        drawTexture("node0-low.png")
    end

    if getNodeState(1) == 1 then
        drawTexture("node1-hi.png")
    elseif getNodeState(1) == 2 then
        drawTexture("node1-low.png")
    end

    if getNodeState(0) == 1 and getNodeState(1) == 2 then
        drawTexture("led.png")
    elseif getNodeState(1) == 1 and getNodeState(0) == 2 then
        drawTexture("led.png")
    end
end

function setAngle(ang)

end