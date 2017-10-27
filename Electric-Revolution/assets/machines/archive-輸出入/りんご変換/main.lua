machine = {
	name = "りんご開封ポート",
	size = {x = 6, y = 3},
	group = "外部接続ポート"
}

i = 0
j = 1

function init()
    setGearbox(0, 1, 10)
    setGearbox(5, 1, 10)
    connectGearbox(0, 1)
end

function updateSystem()

    if isItemPos(4, 1) or isItemPos(5, 1) then
        i = i + 1
        moveItem(4, 1, 0, 0.0625)
        moveItem(5, 1, 0, 0.0625)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    end
    
    if isItemPos(0, 1) then
        moveItem(0, 1, 0, 0.0625)
    end
    if isItemPos(1, 1) and not isItemPos(5, 1) then
        removeItem(1, 1)
        addItem(5, 1, 1)
    end
end

function draw()
    drawTexture("image.png")

    drawTextureAt("conveyor/conveyor"..j..".png", 1, 1, 5, 1)
    drawTextureAt("conveyor/conveyor"..j..".png", 1, 1, 0, 1)
end

function canPutItemAt(x, y, id)
    if (x == 5 and y == 1) then
        return true
    elseif((x == 0 and y == 1) or (x == 1 and y == 1) and id == 8) then
        return true
    else
        return false
    end
end