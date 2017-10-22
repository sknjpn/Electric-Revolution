i = 0
j = 1

function init()
    setNode(0.5, 1.0, 0.0, 1.0)
end

function updateSystem()
    if getNodeState(0) == 2 then
        clearKineticEnergy(4, 1)
        addKineticEnergy(100.0, 4, 1)
        i = i + 1
        if i == 4 then
            i = 0
            j = j + 1
            if j == 6 then
                playAudio("sound.mp3")
            end
            if j == 14 then
                j = 0
                playAudio("sound.mp3")
            end
            if j == 16 then
                j = 0
                playAudio("sound.mp3")
            end
        end
    end
end

function updateConnects()

end

function draw()
    drawTexture("image.png")
    drawTexture("engine"..j..".png")
end