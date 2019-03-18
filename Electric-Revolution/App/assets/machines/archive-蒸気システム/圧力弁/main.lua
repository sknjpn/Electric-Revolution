machine = {
	name = "安全弁",
	size = {x = 1, y = 1},
	group = "蒸気システム"
}

function init()
end

function updateSystem()
    if getPressure(0, 0) > 2.0 then
        pullMole(0, 0, getPressure(0, 0))
    end
end

function draw()
    drawTexture("image.png")
end