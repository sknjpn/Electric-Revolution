machine = {
	name = "分別コンベア",
	size = {x = 2, y = 1},
	group = "コンベア"
}
speed = 0.0625
progress = 0.0

function init()
    setNode(1.5, 0.25, 0.5, 0.0)
    setNode(1.5, 0.75, 0.5, 0.0)
    setGearbox(0, 0, 0)
end

function updateSystem()
    if getNodeState(0) == 2 and getNodeState(1) ~= 2 then
        setGearboxResistance(0, 10)
        
        if getGearboxGain(0) >= 1.0 then
            progress = progress + (speed * 16.0)
            moveItem(0, 0, 3, speed)
        else
            moveItem(0, 0, 3, speed * getGearboxGain(0))
            progress = progress + (speed * 8.0 * getGearboxGain(0) / 1.0)
        end

        if math.ceil(progress) >= 8 then
            progress = progress - 8.0
        end
    elseif getNodeState(1) == 2 and getNodeState(0) ~= 2  then
        setGearboxResistance(0, 10)
        
        if getGearboxGain(0) >= 1.0 then
            moveItem(0, 0, 1, speed)
            progress = progress - (speed * 16.0)
        else
            moveItem(0, 0, 1, speed * getGearboxGain(0))
            progress = progress + (speed * 8.0 * getGearboxGain(0) / 1.0)
        end
        
        if math.ceil(progress) <= -1 then
            progress = progress + 8.0
        end
    else
        setGearboxResistance(0,  0.0)
    end
end

function draw()
    drawTexture("image.png")
    drawTexture("conveyor"..math.ceil(progress)..".png")
end

function canPutItemAt(x, y, id)
    if x == 0 and y == 0 then
        return true
    else
        return false
    end
end