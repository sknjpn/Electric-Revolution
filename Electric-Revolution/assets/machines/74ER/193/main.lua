counter = 0
previousState = 0

function init()
    setNode(0.5, 0.0, 0.0, -2.0, 2, true)
    setNode(1.5, 0.0, 0.0, -2.0, 2, true)
    setNode(2.5, 0.0, 0.0, -2.0, 2, true)
    setNode(3.5, 0.0, 0.0, -2.0, 2, true)
    setNode(0.5, 3.0, 0.0,  2.0)
    setNode(1.5, 3.0, 0.0,  2.0)
    setNode(2.5, 3.0, 0.0,  2.0)
    setNode(3.5, 3.0, 0.0,  2.0)

end

function updateSystem()
    if getNodeState(4) == 1 and previousState ~=1 then
        counter = counter + 1
        
        setNodeState(0, 2)
        setNodeState(1, 2)
        setNodeState(2, 2)
        setNodeState(3, 2)
        if counter & (1 << 0) ~= 0 then setNodeState(0, 1) end
        if counter & (1 << 1) ~= 0 then setNodeState(1, 1) end
        if counter & (1 << 2) ~= 0 then setNodeState(2, 1) end
        if counter & (1 << 3) ~= 0 then setNodeState(3, 1) end
    end
    previousState = getNodeState(4)
end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    for i = 0, 7, 1 do
        if getNodeState(i) == 1 then
            drawTexture("node"..i.."-hi.png")
        end
        if getNodeState(i) == 2 then
            drawTexture("node"..i.."-low.png")
        end
    end
end

function setAngle(ang)

end