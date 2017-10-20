i = 0
j = 1
function init()

end

function updateSystem()

    if isItemPos(3, 1) then
        i = i + 1
        moveItem(3, 1, 4, 1, 0.0156)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    elseif isWallTile(-1, 1) then
        addItem(3, 1, 1)
    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    drawTexture("conveyor"..j..".png", 3, 1, 1, 1)
end