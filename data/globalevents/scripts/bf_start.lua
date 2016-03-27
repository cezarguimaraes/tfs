local TELEPORT_POSITION = Position(92, 114, 7)
local TELEPORT_ACTIONID = 4040
local TELEPORT_ITEMID = 1387

local function warnEvent(i)
	Game.broadcastMessage("O evento Battlefield está aberto!")
	if i > 1 then
		addEvent(warnEvent, 2 * 60 * 1000, i - 1)
	end
end

local function removeTp(position)
	local tp = Tile(position):getItemById(TELEPORT_ITEMID)
	if tp then
		tp:remove(1)
	end
end

local function openBattlefield()
	Battlefield:Open()
end

local function closeBattlefield()
	Battlefield:Close()
end

function onTime(interval)
	Game.broadcastMessage("O evento Battlefield comecara em 10 minutos.")
	local tp = Game.createItem(TELEPORT_ITEMID, 1, TELEPORT_POSITION)
	if not tp then
		error("Nao foi possivel criar o teleport na posicao especificada.")
	end
	tp:setActionId(TELEPORT_ACTIONID)
	addEvent(warnEvent, 2 * 60 * 1000, 4)
	addEvent(removeTp, 10 * 60 * 1000, TELEPORT_POSITION)
	addEvent(openBattlefield, 10 * 60 * 1000)
	addEvent(closeBattlefield, 60 * 60 * 1000)
	return true
end
