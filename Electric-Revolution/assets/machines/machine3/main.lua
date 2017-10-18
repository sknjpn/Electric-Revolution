function init()
    setNode(0.0, 1.5, 0, false)
    setNode(4.0, 1.5, 0, false)
    setNode(1.0, 3.0, 0, false)
    setNode(3.0, 3.0, 0, false)
end

function updateSystem()

end

function updateConnects()

    if (getNodeState(2) == 1 and getNodeState(3) == 2) or
     (getNodeState(2) == 2 and getNodeState(3) == 1) then
        for i = 0, 1, 1 do
            if getNodeState(i) ~= 0 then
                state = getNodeState(i)
                for j = 0, 1, 1 do
                    setNodeState(j, state)
                end
            end
        end
    end
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
    
    if getNodeState(2) == 1 then
        drawTexture("node2-hi.png")
    elseif getNodeState(2) == 2 then
        drawTexture("node2-low.png")
    end

    if getNodeState(3) == 1 then
        drawTexture("node3-hi.png")
    elseif getNodeState(3) == 2 then
        drawTexture("node3-low.png")
    end
    
    if (getNodeState(2) == 1 and getNodeState(3) == 2) or
     (getNodeState(2) == 2 and getNodeState(3) == 1) then
        drawTexture("light.png")
    end
end