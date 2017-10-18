function init()
    setNode(0.0, 1.5, 0, false)
    setNode(4.0, 1.5, 0, false)
    setNode(2.0, 1.5, 2, true)
end

function updateSystem()
    if machineLeftClicked() then
        playAudio("sound.mp3")
        if getNodeState(2) == 1 then
            setNodeState(2, 2)
        else
            setNodeState(2, 1)
        end
    end
end

function updateConnects()
    if getNodeState(2) == 1 then
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

end