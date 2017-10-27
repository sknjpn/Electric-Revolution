machine = {
	name = "74ER08",
	size = {x = 3, y = 2},
	group = "汎用ロジックIC"
}

function init()
    --bottom left
    setNode(0.375, 1.75, 0.0,  0.5)    --0  1A
    setNode(0.750, 1.75, 0.0,  0.5)    --1  1B
    setNode(1.125, 1.75, 0.0,  0.5)    --2  1Y
    setNode(1.500, 1.75, 0.0,  0.5)    --3  2A
    setNode(1.875, 1.75, 0.0,  0.5)    --4  2B
    setNode(2.250, 1.75, 0.0,  0.5)    --5  2Y
    setNode(2.625, 1.75, 0.0,  0.5)    --6  GND

    setNode(2.625, 0.25, 0.0, -0.5)    --7  3Y
    setNode(2.250, 0.25, 0.0, -0.5)    --8  3A
    setNode(1.875, 0.25, 0.0, -0.5)    --9  3B
    setNode(1.500, 0.25, 0.0, -0.5)    --10 4Y
    setNode(1.125, 0.25, 0.0, -0.5)    --11 4A
    setNode(0.750, 0.25, 0.0, -0.5)    --12 4B
    setNode(0.375, 0.25, 0.0, -0.5)    --13 VCC
end

function updateSystem()

    --VCC and GND
    if getNodeState(6) == 2 and getNodeState(13) == 1 then

        --setFixed
        setNodeFixed( 2, true)
        setNodeFixed( 5, true)
        setNodeFixed( 7, true)
        setNodeFixed(10, true)

        --1 AND
        if getNodeState(0) ~= 2 and getNodeState(1) ~= 2 then
            setNodeState( 2, 1)
        else
            setNodeState( 2, 2)
        end
        
        --2 AND
        if getNodeState(3) ~= 2 and getNodeState(4) ~= 2 then
            setNodeState( 5, 1)
        else
            setNodeState( 5, 2)
        end
        
        --3 AND
        if getNodeState(8) ~= 2 and getNodeState(9) ~= 2 then
            setNodeState( 7, 1)
        else
            setNodeState( 7, 2)
        end
        
        --4 AND
        if getNodeState(11) ~= 2 and getNodeState(12) ~= 2 then
            setNodeState(10, 1)
        else
            setNodeState(10, 2)
        end

    else

        --resetFixed
        setNodeFixed( 2, false)
        setNodeFixed( 5, false)
        setNodeFixed( 7, false)
        setNodeFixed(10, false)

    end

end

function updateConnects()

end

function draw()
    drawTexture("image.png")
end