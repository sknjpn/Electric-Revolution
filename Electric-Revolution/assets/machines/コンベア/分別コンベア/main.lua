speed = 0.100
progress = 0.0

function init()
    setNode(1.5, 0.25, 0.5, 0.0)
    setNode(1.5, 0.75, 0.5, 0.0)
end

function updateSystem()
    
    if getNodeState(0) == 2 and getNodeState(1) ~= 2 then

        progress = progress + (speed * 16.0)
        if math.ceil(progress) == 8 then
            progress = progress - 8.0
        end

        moveItem(0, 0, 3, speed)

    elseif getNodeState(1) == 2 and getNodeState(0) ~= 2  then

        progress = progress - (speed * 16.0)
        if math.ceil(progress) == -1 then
            progress = progress + 8.0
        end

        moveItem(0, 0, 1, speed)

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