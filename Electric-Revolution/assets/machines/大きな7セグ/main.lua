function init()
    setNode(0.5, 8.0, 0.0, 2.0)
    setNode(1.5, 8.0, 0.0, 2.0)
    setNode(2.5, 8.0, 0.0, 2.0)
    setNode(3.5, 8.0, 0.0, 2.0)
end

function updateSystem()

end

function updateConnects()

end

function draw()
    drawTexture("image.png")

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
    if ((result & (1 << 0)) ~= 0) then drawTexture("light-a.png") end
    if ((result & (1 << 1)) ~= 0) then drawTexture("light-b.png") end
    if ((result & (1 << 2)) ~= 0) then drawTexture("light-c.png") end
    if ((result & (1 << 3)) ~= 0) then drawTexture("light-d.png") end
    if ((result & (1 << 4)) ~= 0) then drawTexture("light-e.png") end
    if ((result & (1 << 5)) ~= 0) then drawTexture("light-f.png") end
    if ((result & (1 << 6)) ~= 0) then drawTexture("light-g.png") end
end

function setAngle(ang)

end