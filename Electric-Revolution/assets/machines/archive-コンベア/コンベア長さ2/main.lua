machine = {
	name = "Conveyor2",
	size = {x = 2, y = 1},
	group = "コンベア"
}

speed = 0.0625
progress = 0.0

function init()
    setGearbox(0, 0, 20)
    setGearbox(1, 0,  0)
    connectGearbox(0, 1)
end

function updateSystem()

    if getGearboxGain(0) >= 2.0 then
        for i = 0, 1 do moveItem(i, 0, 0, speed) end
        progress = progress + (speed * 8.0)
        if math.ceil(progress) == 8 then
            progress = progress - 8.0
        end
    else
        for i = 0, 1 do moveItem(i, 0, 0, speed * getGearboxGain(0) / 2.0) end
        progress = progress + (speed * 8.0 * getGearboxGain(0) / 2.0)
        if math.ceil(progress) == 8 then
            progress = progress - 8.0
        end
    end

end

function draw()
    drawTexture("image.png")
    drawTexture("conveyor"..math.ceil(progress)..".png")
end

function canPutItemAt(x, y, id)
    return true
end