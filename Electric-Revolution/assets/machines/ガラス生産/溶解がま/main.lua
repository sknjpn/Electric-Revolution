ir = 0
jr = 1
il = 0
jl = 1
progress = 0
fire = 1500
before = 0
after = 0
function init()
    
end

function updateSystem()

    fire = fire + 10
    if before >= 2 and after <= 1000 then
        before = before - 2
        after = after + 1

        if fire > 1800 then
            playAudio("sound.mp3")
            fire = 0
        end
    end
    
    if after >= 100 then
        addItem(4, 2, 3)
        after = after - 100    
    end


    --Import Left
    if isItemPos(0, 2) and not isItemPos(1, 2) then
        il = il + 1
        moveItem(0, 2, 0, 0.0625)
        if il == 4 then
            il = 0
            jl = jl + 1
            if jl == 9 then
                jl = 1
            end
        end
    end
    if before <= 900 and isItemPos(1, 2) and not isItemPos(4, 2) then
        removeItem(1, 2)
        before = before + 100
    end

    --Export Right
    if isItemPos(4, 2) or isItemPos(5, 2) then
        ir = ir + 1
        if ir == 4 then
            ir = 0
            jr = jr + 1
            if jr == 9 then
                jr = 1
            end
        end
        moveItem(5, 2, 0, 0.0625)
        moveItem(4, 2, 0, 0.0625)
    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")

    drawTextureAt("conveyor/conveyor"..jr..".png", 1, 1, 5, 2)
    drawTextureAt("conveyor/conveyor"..jl..".png", 1, 1, 0, 2)
end

function canPutItemAt(x, y, id)
    if  (((x == 0 and y == 2) or (x == 1 and y == 2)) and id == 2) or
        ((x == 5 and y == 2) and (id == 3)) then
        return true
    else
        return false
    end
end