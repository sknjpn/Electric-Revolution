function init()
    setNode(1.5, 0.0,  0.0, -2.0)
    setNode(3.0, 1.5,  2.0,  0.0)
    setNode(1.5, 3.0,  0.0,  2.0)
    setNode(0.0, 1.5, -2.0,  0.0)
end

function updateSystem()

end

function updateConnects()
    for i = 0, 3, 1 do
        if getNodeState(i) ~= 0 then
            state = getNodeState(i)
            for j = 0, 3, 1 do
                setNodeState(j, state)
            end
        end
    end
end

function draw()
    drawTexture("image.png")
    if getNodeState(0) == 1 then
        drawTexture("node-hi.png")
    end
    if getNodeState(0) == 2 then
        drawTexture("node-low.png")
    end
end