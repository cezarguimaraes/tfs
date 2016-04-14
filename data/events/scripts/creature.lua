function Creature:onChangeOutfit(outfit)
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return true
end

local staminaBonus = {
	target = 'Trainer Name',
	period = 30000, -- período em milisegundos
	bonus = 5, -- stamina que ganha
	events = {}
}

local function addStamina(name)
	local player = Player(name)
	if not player then
		staminaBonus.events[name] = nil
	else
		local target = player:getTarget()
		if not target or target:getName() ~= staminaBonus.target then
			staminaBonus.events[name] = nil
		else
			player:setStamina(player:getStamina() + staminaBonus.bonus)
			staminaBonus.events[name] = addEvent(addStamina, staminaBonus.period, name) 
		end
	end
end

function Creature:onTargetCombat(target)
	if self:isPlayer() then
		if target and target:getName() == staminaBonus.target then
			local name = self:getName()
			if not staminaBonus.events[name] then
				staminaBonus.events[name] = addEvent(addStamina, staminaBonus.period, name)
			end
		end
	end
	return true
end
