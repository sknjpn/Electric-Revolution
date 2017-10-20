function init()
    --bottom left
    setNode(0.375, 1.75, 0.0,  0.25)    --0  1A
    setNode(0.750, 1.75, 0.0,  0.25)    --1  1Y
    setNode(1.125, 1.75, 0.0,  0.25)    --2  2A
    setNode(1.500, 1.75, 0.0,  0.25)    --3  2Y
    setNode(1.875, 1.75, 0.0,  0.25)    --4  3A
    setNode(2.250, 1.75, 0.0,  0.25)    --5  3Y
    setNode(2.625, 1.75, 0.0,  0.25)    --6  GND

    setNode(2.625, 0.25, 0.0, -0.25)    --7  4Y
    setNode(2.250, 0.25, 0.0, -0.25)    --8  4A
    setNode(1.875, 0.25, 0.0, -0.25)    --9  5Y
    setNode(1.500, 0.25, 0.0, -0.25)    --10 5A
    setNode(1.125, 0.25, 0.0, -0.25)    --11 6Y
    setNode(0.750, 0.25, 0.0, -0.25)    --12 6A
    setNode(0.375, 0.25, 0.0, -0.25)    --13 VCC
end

function updateSystem()

    --VCC and GND
    if getNodeState(6) == 2 and getNodeState(13) == 1 then

        --setFixed
        setNodeFixed( 1, true)
        setNodeFixed( 3, true)
        setNodeFixed( 5, true)
        setNodeFixed( 7, true)
        setNodeFixed( 9, true)
        setNodeFixed(11, true)

        --1 NOT
        if getNodeState( 0) ~= 2 then
            setNodeState( 1, 2)
        else
            setNodeState( 1, 1)
        end
        
        --2 NOT
        if getNodeState( 2) ~= 2 then
            setNodeState( 3, 2)
        else
            setNodeState( 3, 1)
        end
        
        --3 NOT
        if getNodeState( 4) ~= 2 then
            setNodeState( 5, 2)
        else
            setNodeState( 5, 1)
        end
        
        --4 NOT
        if getNodeState( 8) ~= 2 then
            setNodeState( 7, 2)
        else
            setNodeState( 7, 1)
        end
        
        --5 NOT
        if getNodeState(10) ~= 2 then
            setNodeState( 9, 2)
        else
            setNodeState( 9, 1)
        end
        
        --6 NOT
        if getNodeState(12) ~= 2 then
            setNodeState(11, 2)
        else
            setNodeState(11, 1)
        end

    else

        --resetFixed
        setNodeFixed( 1, false)
        setNodeFixed( 3, false)
        setNodeFixed( 5, false)
        setNodeFixed( 7, false)
        setNodeFixed( 9, false)
        setNodeFixed(11, false)

    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")
end