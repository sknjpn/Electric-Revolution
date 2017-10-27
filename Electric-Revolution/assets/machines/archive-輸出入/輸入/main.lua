machine = {
	name = "輸入ポート",
	size = {x = 5, y = 3},
	group = "外部接続ポート"
}

i = 0
j = 1

function updateSystem()

    if isItemPos(3, 1) or isItemPos(3, 1) then
        i = i + 1
        moveItem(3, 1, 0, 0.0625)
        moveItem(4, 1, 0, 0.0625)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    elseif isTradeTile(-1, 1) then
        addItem(3, 1, 8)
    end

end

function draw()
    drawTexture("image.png")

    drawTextureAt("conveyor/conveyor"..j..".png", 1, 1, 4, 1)
end

function canPutItemAt(x, y, id)
    if (x == 4 and y == 1) then
        return true
    else
        return false
    end
end