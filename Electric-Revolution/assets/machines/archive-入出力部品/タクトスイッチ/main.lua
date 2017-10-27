machine = {
	name = "タクトスイッチ",
	size = {x = 1, y = 1},
	group = "入出力部品"
}

pushed = false

function init()
    setNode(0.125, 0.5, -1.0, 0.0)
    setNode(0.875, 0.5,  1.0, 0.0)
end

function updateConnects()
    if pushed then
        if getNodeState(0) ~= 0 then
            setNodeState(1, getNodeState(0))
        elseif getNodeState(2) ~= 0 then
            setNodeState(0, getNodeState(1))
        end
    end
end

function draw()
    
    if pushed ~= machineLeftPressed() then
        playAudio("sound.mp3")
        pushed = machineLeftPressed()
    end

    drawTexture("image.png")
    
    if machineLeftPressed() then
        drawTexture("push-d.png")
    else
        drawTexture("push-u.png")
    end
end