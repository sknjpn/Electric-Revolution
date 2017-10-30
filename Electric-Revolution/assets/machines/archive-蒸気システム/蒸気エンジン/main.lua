machine = {
	name = "蒸気エンジン",
	size = {x = 6, y = 1},
	group = "蒸気システム"
}

progress = 0.0
image = 0

function init()
    setNode(0.5, 1.0, 0.0, 1.0)
    setGearbox(4, 0, 10.0)
end

function updateSystem()
    if getPressure(0, 0) > 0.0 then
        
        addGearboxPressure(0, getPressure(0, 0) * 20)
        progress = progress + getPressure(0, 0)

        pullMole(0, 0, getPressure(0, 0))
        
        while progress >= 1.0 do
            progress = progress - 1.0
            image = image + 1
            if image == 16 then image = 0 end
            if image == 0 then playAudio("sound.wav") end
            if image == 8 then playAudio("sound.wav") end
        end
    end
end

function draw()
    drawTexture("image.png")
    drawTexture("engine"..image..".png")
end