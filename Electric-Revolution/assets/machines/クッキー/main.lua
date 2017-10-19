energy = 0
i = 0
j = 1
function init()
    setNode(1.0, 3.0, 0.0, 2.0)
end

function updateSystem()
    if getNodeState(0) == 1 then
        if energy == 360 then
            if not isItemPos(3, 1) then
                energy = 0
                addItem(3, 1)
            end
        else
            if energy == 0 then playAudio("sound1.mp3") end
            if energy == 60 then playAudio("sound1.mp3") end
            if energy == 120 then playAudio("sound1.mp3") end
            if energy == 180 then playAudio("sound1.mp3") end
            if energy == 240 then playAudio("sound1.mp3") end
            if energy == 300 then playAudio("sound1.mp3") end
            energy = energy + 2
        end
    end
    
    
    if isItemPos(3, 1) then
        i = i + 1
        moveItem(3, 1, 4, 1, 0.0156)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    end
end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    drawTexture("conveyor"..j..".png", 3, 1, 1, 1)

    if getNodeState(0) == 1 then
        drawTexture("node-hi.png")
    elseif getNodeState(0) == 2 then
        drawTexture("node-low.png")
    end

    if energy > 60 then
        if energy > 120 then
            if energy > 180 then
                if energy > 240 then
                    if energy > 300 then
                        drawTexture("energy5.png")
                    else
                        drawTexture("energy4.png")
                    end
                else
                    drawTexture("energy3.png")
                end
            else
                drawTexture("energy2.png")
            end
        else
            drawTexture("energy1.png")
        end
    end
end

function setAngle(ang)

end