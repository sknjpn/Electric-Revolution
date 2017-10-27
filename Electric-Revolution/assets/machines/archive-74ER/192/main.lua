machine = {
	name = "74ER192",
	size = {x = 4, y = 2},
	group = "汎用ロジックIC"
}

function init()
    --bottom left
    setNode(0.6875, 1.75, 0.0,  0.5)    --0  B
    setNode(1.0625, 1.75, 0.0,  0.5)    --1  QB
    setNode(1.4375, 1.75, 0.0,  0.5)    --2  QA
    setNode(1.8125, 1.75, 0.0,  0.5)    --3  COUNT DOWN
    setNode(2.1875, 1.75, 0.0,  0.5)    --4  COUNT UP
    setNode(2.5625, 1.75, 0.0,  0.5)    --5  QC
    setNode(2.9375, 1.75, 0.0,  0.5)    --6  QD
    setNode(3.3125, 1.75, 0.0,  0.5)    --7  GND

    setNode(3.3125, 0.25, 0.0, -0.5)    --8  D
    setNode(2.9375, 0.25, 0.0, -0.5)    --9  C
    setNode(2.5625, 0.25, 0.0, -0.5)    --10 LOAD
    setNode(2.1875, 0.25, 0.0, -0.5)    --11 CARRY
    setNode(1.8125, 0.25, 0.0, -0.5)    --12 BORROW
    setNode(1.4375, 0.25, 0.0, -0.5)    --13 CLR
    setNode(1.0625, 0.25, 0.0, -0.5)    --14 A
    setNode(0.6875, 0.25, 0.0, -0.5)    --15 VCC
end

register = 0
previousCountDown = 0
previousCountUp = 0

function updateSystem()

    --VCC and GND
    if getNodeState(7) == 2 and getNodeState(15) == 1 then

        --setFixed
        setNodeFixed( 1, true)
        setNodeFixed( 2, true)
        setNodeFixed( 5, true)
        setNodeFixed( 6, true)
        setNodeFixed(11, true)
        setNodeFixed(12, true)

        --11 CARRY
        setNodeState(11, 1)
        
        --12 BORROW
        setNodeState(12, 1)
        
        --13 CLR
        if getNodeState(13) ~= 2 then
            register = 0
        elseif getNodeState(10) == 2 then   --10 LOAD
            register = 0
            if getNodeState(14) ~= 2 then register = register + 1 end
            if getNodeState( 0) ~= 2 then register = register + 2 end
            if getNodeState( 9) ~= 2 then register = register + 4 end
            if getNodeState( 8) ~= 2 then register = register + 8 end
        else 
            --3  COUNT DOWN
            if getNodeState(4) ~= 2 and getNodeState(3) ~= 2 and previousCountDown == 2 then
                if register == 0 then
                    register = 9
                else
                    register = register - 1
                end
            end
            previousCountDown = getNodeState(3)
        
            --4  COUNT UP
            if getNodeState(3) ~= 2 and getNodeState(4) ~= 2 and previousCountUp == 2 then
                if register == 9 then
                    register = 0
                elseif register == 11 then
                    register = 6
                elseif register == 13 then
                    register = 4
                elseif register == 15 then
                    register = 2
                else
                    register = register + 1
                end
            end
            previousCountUp = getNodeState(4)

            --11 CARRY
            if register ==  9 and getNodeState(3) ~= 2 and getNodeState(4) == 2 then
                setNodeState(11, 2)
            end
            --12 BORROW
            if register ==  0 and getNodeState(4) ~= 2 and getNodeState(3) == 2 then
                setNodeState(12, 2)
            end
        end

        if (register & (1 << 0)) ~= 0 then setNodeState(2, 1) else setNodeState(2, 2) end
        if (register & (1 << 1)) ~= 0 then setNodeState(1, 1) else setNodeState(1, 2) end
        if (register & (1 << 2)) ~= 0 then setNodeState(5, 1) else setNodeState(5, 2) end
        if (register & (1 << 3)) ~= 0 then setNodeState(6, 1) else setNodeState(6, 2) end
        
    else
        register = 0    --Reset

        --resetFixed
        setNodeFixed( 1, false)
        setNodeFixed( 2, false)
        setNodeFixed( 5, false)
        setNodeFixed( 6, false)
        setNodeFixed(11, false)
        setNodeFixed(12, false)

    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")
end