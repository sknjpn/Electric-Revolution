function init()
    --bottom left
    setNode(0.375, 1.75, 0.0,  0.5)    --0  A
    setNode(0.750, 1.75, 0.0,  0.5)    --1  B
    setNode(1.125, 1.75, 0.0,  0.5)    --2  QA
    setNode(1.500, 1.75, 0.0,  0.5)    --3  QB
    setNode(1.875, 1.75, 0.0,  0.5)    --4  QC
    setNode(2.250, 1.75, 0.0,  0.5)    --5  QD
    setNode(2.625, 1.75, 0.0,  0.5)    --6  GND

    setNode(2.625, 0.25, 0.0, -0.5)    --7  CLK
    setNode(2.250, 0.25, 0.0, -0.5)    --8  CLR
    setNode(1.875, 0.25, 0.0, -0.5)    --9  QE
    setNode(1.500, 0.25, 0.0, -0.5)    --10 QF
    setNode(1.125, 0.25, 0.0, -0.5)    --11 QG
    setNode(0.750, 0.25, 0.0, -0.5)    --12 QH
    setNode(0.375, 0.25, 0.0, -0.5)    --13 VCC
end

previousClockState = 2

function updateSystem()

    --VCC and GND
    if getNodeState(6) == 2 and getNodeState(13) == 1 then

        --setFixed
        setNodeFixed( 2, true)
        setNodeFixed( 3, true)
        setNodeFixed( 4, true)
        setNodeFixed( 5, true)
        setNodeFixed( 9, true)
        setNodeFixed(10, true)
        setNodeFixed(11, true)
        setNodeFixed(12, true)
        if getNodeState( 2) == 0 then setNodeState( 2, 2) end
        if getNodeState( 3) == 0 then setNodeState( 3, 2) end
        if getNodeState( 4) == 0 then setNodeState( 4, 2) end
        if getNodeState( 5) == 0 then setNodeState( 5, 2) end
        if getNodeState( 9) == 0 then setNodeState( 9, 2) end
        if getNodeState(10) == 0 then setNodeState(10, 2) end
        if getNodeState(11) == 0 then setNodeState(11, 2) end
        if getNodeState(12) == 0 then setNodeState(12, 2) end

        if getNodeState(8) == 2 then
            setNodeState( 2, 2)
            setNodeState( 3, 2)
            setNodeState( 4, 2)
            setNodeState( 5, 2)
            setNodeState( 9, 2)
            setNodeState(10, 2)
            setNodeState(11, 2)
            setNodeState(12, 2)
        elseif getNodeState(7) ~= 2 and previousClockState == 2 then
            setNodeState(12, getNodeState(11))
            setNodeState(11, getNodeState(10))
            setNodeState(10, getNodeState( 9))
            setNodeState( 9, getNodeState( 5))
            setNodeState( 5, getNodeState( 4))
            setNodeState( 4, getNodeState( 3))
            setNodeState( 3, getNodeState( 2))
            if getNodeState(1) ~= 2 and getNodeState(1) ~= 2 then
                setNodeState(2, 1)
            else
                setNodeState(2, 2)
            end
        end
    else

        --resetFixed
        setNodeFixed( 2, false)
        setNodeFixed( 3, false)
        setNodeFixed( 4, false)
        setNodeFixed( 5, false)
        setNodeFixed( 9, false)
        setNodeFixed(10, false)
        setNodeFixed(11, false)
        setNodeFixed(12, false)

    end
    previousClockState = getNodeState(7)
end

function updateConnects()

end

function draw()
    drawTexture("image.png")
end