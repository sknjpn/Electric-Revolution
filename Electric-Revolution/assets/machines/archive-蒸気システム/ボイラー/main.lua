machine = {
	name = "ボイラー",
	size = {x = 8, y = 5},
	group = "蒸気システム"
}
i = 0
j = 0

fuel = 0

function updateSystem()

    if fuel > 0 then
        fuel = fuel - 1
        addMole(0, 3, 4.0)
    end

    --Import Left
    if isItemPos(0, 2) and not isItemPos(1, 2) and fuel == 0 then
        i = i + 1
        moveItem(0, 2, 0, 0.0625)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 8 then
                j = 0
            end
        end
    end

    if isItemPos(1, 2) then
        removeItem(1, 2)
        fuel = 300
        playAudio("sound.mp3")
    end
end

function draw()
    drawTexture("image.png")

    drawTextureAt("conveyor/conveyor"..j..".png", 1, 1, 0, 2)
end

function canPutItemAt(x, y, id)
    if  (((x == 0 and y == 2) or (x == 1 and y == 2)) and id == 9) then
        return true
    else
        return false
    end
end