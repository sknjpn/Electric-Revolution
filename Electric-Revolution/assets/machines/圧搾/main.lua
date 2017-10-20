ir = 0
jr = 1
il = 0
jl = 1
progress = 0
function init()

end

function updateSystem()

    --Progress
    if progress > 0 then
        progress = progress + 1
        if progress == 120 then
            addItem(3, 1, 2)
            progress = 0
        end
    end

    --Import Left
    if isItemPos(0, 1) and not isItemPos(1, 1) then
        il = il + 1
        moveItem(0, 1, 1, 1, 0.0156)
        if il == 4 then
            il = 0
            jl = jl + 1
            if jl == 9 then
                jl = 1
            end
        end
    end
    if progress == 0 and isItemPos(1, 1) and not isItemPos(3, 1) then
        removeItem(1, 1)
        playAudio("sound.mp3")
        progress = 1
    end

    --Export Right
    if isItemPos(3, 1) then
        ir = ir + 1
        moveItem(3, 1, 4, 1, 0.0156)
        if ir == 4 then
            ir = 0
            jr = jr + 1
            if jr == 9 then
                jr = 1
            end
        end
    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    drawTexture("cr/conveyor"..jr..".png", 3, 1, 1, 1)
    drawTexture("cl/conveyor"..jl..".png", 3, 1, 1, 1)
end