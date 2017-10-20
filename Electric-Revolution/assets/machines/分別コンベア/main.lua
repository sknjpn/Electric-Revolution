energy = 0
i = 0
j = 1
function init()
    setNode(1.5, 0.25, 0.5, 0.0)
    setNode(1.5, 0.75, 0.5, 0.0)
end

function updateSystem()
    
    if getNodeState(0) == 2 and getNodeState(1) ~= 2 then
        i = i + 1
        moveItem(0, 0, 0, -1, 0.0156)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    elseif getNodeState(1) == 2 and getNodeState(0) ~= 2  then
        i = i + 1
        moveItem(0, 0, 0, 1, 0.0156)
        if i == 4 then
            i = 0
            j = j + 1
            if j == 9 then
                j = 1
            end
        end
    end
end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    if getNodeState(0) == 2 and getNodeState(1) ~= 2 then
        drawTexture("cu/conveyor"..j..".png", 3, 1, 1, 1)
    else
        drawTexture("cd/conveyor"..j..".png", 3, 1, 1, 1)
    end
end