i = 0
j = 1

function init()

end

function updateSystem()
    if isItemPos(7, 0) or isItemPos(6, 0) or isItemPos(5, 0) or isItemPos(4, 0) or isItemPos(3, 0) or isItemPos(2, 0) or isItemPos(1, 0) or isItemPos(0, 0) then
        i = i + 1
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    end

    
    moveItem(7, 0, 8, 0, 0.0156)
    moveItem(6, 0, 7, 0, 0.0156)
    moveItem(5, 0, 6, 0, 0.0156)
    moveItem(4, 0, 5, 0, 0.0156)
    moveItem(3, 0, 4, 0, 0.0156)
    moveItem(2, 0, 3, 0, 0.0156)
    moveItem(1, 0, 2, 0, 0.0156)
    moveItem(0, 0, 1, 0, 0.0156)
end

function updateConnects()

end

function draw()
    drawTexture("image.png")
    drawTexture("conveyor"..j..".png")
end

function setAngle(ang)

end