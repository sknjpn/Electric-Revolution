function init()
    setNode(1.1, 3.0, 0.0, 2.0)
    setNode(1.7, 3.0, 0.0, 2.0)
    setNode(2.3, 3.0, 0.0, 2.0)
    setNode(2.9, 3.0, 0.0, 2.0)
end

function updateSystem()

end

function updateConnects()

end

function draw()
    drawTexture("image.png")
    
    if getNodeState(0) == 1 then
        drawTexture("node0-hi.png")
    elseif getNodeState(0) == 2 then
        drawTexture("node0-low.png")
    end

    if getNodeState(1) == 1 then
        drawTexture("node1-hi.png")
    elseif getNodeState(1) == 2 then
        drawTexture("node1-low.png")
    end
    if getNodeState(2) == 1 then
        drawTexture("node2-hi.png")
    elseif getNodeState(2) == 2 then
        drawTexture("node2-low.png")
    end

    if getNodeState(3) == 1 then
        drawTexture("node3-hi.png")
    elseif getNodeState(3) == 2 then
        drawTexture("node3-low.png")
    end

    bcd = 0
    if getNodeState(0) == 1 then bcd = bcd + 1 end
    if getNodeState(1) == 1 then bcd = bcd + 2 end
    if getNodeState(2) == 1 then bcd = bcd + 4 end
    if getNodeState(3) == 1 then bcd = bcd + 8 end

    if bcd == 0x0 then result = 63 end
    if bcd == 0x1 then result = 6 end
    if bcd == 0x2 then result = 91 end
    if bcd == 0x3 then result = 79 end
    if bcd == 0x4 then result = 102 end
    if bcd == 0x5 then result = 109 end
    if bcd == 0x6 then result = 125 end
    if bcd == 0x7 then result = 39 end
    if bcd == 0x8 then result = 127 end
    if bcd == 0x9 then result = 111 end
    if bcd == 0xA then result = 119 end
    if bcd == 0xB then result = 124 end
    if bcd == 0xC then result = 88 end
    if bcd == 0xD then result = 94 end
    if bcd == 0xE then result = 121 end
    if bcd == 0xF then result = 113 end
    if ((result & (1 << 0)) ~= 0) then drawTexture("led-a.png") end
    if ((result & (1 << 1)) ~= 0) then drawTexture("led-b.png") end
    if ((result & (1 << 2)) ~= 0) then drawTexture("led-c.png") end
    if ((result & (1 << 3)) ~= 0) then drawTexture("led-d.png") end
    if ((result & (1 << 4)) ~= 0) then drawTexture("led-e.png") end
    if ((result & (1 << 5)) ~= 0) then drawTexture("led-f.png") end
    if ((result & (1 << 6)) ~= 0) then drawTexture("led-g.png") end
end

function setAngle(ang)

end