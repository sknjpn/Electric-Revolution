i = 0
j = 1

function init()

end

function updateSystem()
    i = i + 1
    if i == 4 then
        i = 0
        j = j + 1
        if j == 9 then
            j = 1
        end
    end
    
    moveItem(0, 0, -1, 0, 0.0156)
    moveItem(1, 0, 0, 0, 0.0156)
end

function updateConnects()

end

function draw()
    drawTexture("image.png")
    drawTexture("conveyor"..j..".png")
end