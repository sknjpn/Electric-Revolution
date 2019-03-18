machine = {
	name = "冷却器",
	size = {x = 4, y = 1},
	group = "汎用マシン"
}

ir = 0
jr = 1
il = 0
jl = 1
progress = 0
function updateSystem()

    --Progress
    if progress > 0 then
        progress = progress + 1
        if progress == 30 then
            addItem(2, 0, 5)
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
        playAudio("sound.mp3")
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

    drawTextureAt("conveyor/conveyor"..jr..".png", 1, 1, 3, 0)
    drawTextureAt("conveyor/conveyor"..jl..".png", 1, 1, 0, 0)
end

function canPutItemAt(x, y, id)
    if  (((x == 0 and y == 0) or (x == 1 and y == 0)) and (id == 4)) or
        ((x == 3 and y == 0) and (id == 5)) then
        return true
    else
        return false
    end
end