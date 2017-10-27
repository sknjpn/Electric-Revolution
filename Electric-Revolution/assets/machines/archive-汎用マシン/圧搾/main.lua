machine = {
	name = "圧縮瓶詰機",
	size = {x = 4, y = 3},
	group = "汎用マシン"
}

function init()
    setGearbox(2, 0, 100)

    setGearbox(0, 1, 10)
    setGearbox(1, 2, 10)
    setGearbox(3, 1, 10)
    
    connectGearbox(1, 2)
    connectGearbox(1, 3)
end

ir = 0
jr = 1
progress = 0
function updateSystem()

    --Progress
    if progress > 0 and getGearboxGain(0) > 40 then
        progress = progress + 1
        if progress == 120 then
            addItem(2, 1, 6)
            progress = 0
        end
    end


    --Import Left
    if progress == 0 and isItemPos(0, 1) and isItemPos(1, 2) then
        removeItem(0, 1)
        removeItem(1, 2)
        playAudio("sound.mp3")
        progress = 1
    end

    --Export Right
    if isItemPos(2, 1) or isItemPos(3, 1) then
        ir = ir + 1
        if ir == 4 then
            ir = 0
            jr = jr + 1
            if jr == 9 then
                jr = 1
            end
        end
        moveItem(3, 1, 0, 0.0625)
        moveItem(2, 1, 0, 0.0625)
    end

end

function draw()
    drawTexture("image.png")

    drawTextureAt("conveyor/conveyor"..jr..".png", 1, 1, 3, 1)
end

function canPutItemAt(x, y, id)
    if  ((x == 0 and y == 1) and (id == 1)) or
        ((x == 1 and y == 2) and (id == 5)) or
        ((x == 3 and y == 1) and (id == 6)) then
        return true
    else
        return false
    end
end