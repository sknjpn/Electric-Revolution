machine = {
	name = "Conveyor1",
	size = {x = 1, y = 1},
	group = "コンベア"
}
speed = 0.0625
progress = 0.0

function init()
    setGearbox(0, 0, 10)
end

function updateSystem()

    if getGearboxGain(0) > 1.0 then
        moveItem(0, 0, 0, speed)
        progress = progress + (speed * 8.0)
        if math.ceil(progress) == 8 then
            progress = progress - 8.0
        end
    else
        moveItem(0, 0, 0, speed * getGearboxGain(0))
        progress = progress + (speed * 8.0 * getGearboxGain(0) / 1.0)
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