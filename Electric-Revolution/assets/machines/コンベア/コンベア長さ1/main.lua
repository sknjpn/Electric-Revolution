speed = 0.0625
progress = 0.0

function updateSystem()

    if isItemPos(0, 0) then
        progress = progress + (speed * 16.0)
        if math.ceil(progress) == 8 then
            progress = progress - 8.0
        end
    end
    
    moveItem(0, 0, 0, speed)
end

function draw()
    drawTexture("image.png")
    drawTexture("conveyor"..math.ceil(progress)..".png")
end

function canPutItemAt(x, y, id)
    return true
end