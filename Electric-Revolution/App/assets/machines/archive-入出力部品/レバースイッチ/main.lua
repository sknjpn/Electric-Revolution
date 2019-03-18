machine = {
	name = "レバースイッチ",
	size = {x = 2, y = 1},
	group = "入出力部品"
}

lever = "right"

function init()
    setNode(0.5, 0.25, 0.0, -1.0)
    setNode(1.0, 0.25, 0.0, -1.0)
    setNode(1.5, 0.25, 0.0, -1.0)
end

function updateConnects()
    if lever == "right" then
        if getNodeState(1) ~= 0 then
            setNodeState(2, getNodeState(1))
        elseif getNodeState(2) ~= 0 then
            setNodeState(1, getNodeState(2))
        end
    else
        if getNodeState(0) ~= 0 then
            setNodeState(1, getNodeState(0))
        elseif getNodeState(1) ~= 0 then
            setNodeState(0, getNodeState(1))
        end
    end
end

function draw()
    if machineLeftClicked() then
        playAudio("sound.mp3")
        if lever == "right" then
            lever = "left"
        else
            lever = "right"
        end
    end
    
    drawTexture("image.png")
    
    if lever == "right" then
        drawTexture("lever-r.png")
    else
        drawTexture("lever-l.png")
    end
end