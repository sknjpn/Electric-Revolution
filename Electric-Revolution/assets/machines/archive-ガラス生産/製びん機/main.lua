ir = 0
jr = 1
il = 0
jl = 1
progress = 0
function updateSystem()

    --Progress
    if progress > 0 and getKineticEnergy(1, 0) > 50 then
        if progress == 1 then playAudio("sound.mp3") end
        progress = progress + 1
        if progress == 120 then
            addItem(2, 0, 4)
            progress = 0
        end
    end


    --Import Left
    if isItemPos(0, 0) and not isItemPos(1, 0) then
        il = il + 1
        moveItem(0, 0, 0, 0.0625)
        if il == 4 then
            il = 0
            jl = jl + 1
            if jl == 9 then
                jl = 1
            end
        end
    end
    if progress == 0 and isItemPos(1, 0) and not isItemPos(2, 0) then
        removeItem(1, 0)
        progress = 1
    end

    --Export Right
    if isItemPos(2, 0) or isItemPos(3, 0) then
        ir = ir + 1
        if ir == 4 then
            ir = 0
            jr = jr + 1
            if jr == 9 then
                jr = 1
            end
        end
        moveItem(3, 0, 0, 0.0625)
        moveItem(2, 0, 0, 0.0625)
    end

end

function draw()
    drawTexture("image.png")

    if getKineticEnergy(1, 0) > 50 then
        drawTexture("progress.png")
    end

    drawTextureAt("conveyor/conveyor"..jr..".png", 1, 1, 3, 0)
    drawTextureAt("conveyor/conveyor"..jl..".png", 1, 1, 0, 0)
end

function canPutItemAt(x, y, id)
    if  (((x == 0 and y == 0) or (x == 1 and y == 0)) and (id == 3)) or
        ((x == 3 and y == 0) and (id == 4)) then
        return true
    else
        return false
    end
end