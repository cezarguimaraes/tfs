currentExpBonus = 1
local bonusExpStages = {
	{1, 1.04}, -- 4% bonus
	{2, 1.05},
	{4, 1.07},
}

local bonusDays = {
	[1] = true, -- Domingo
	[7] = true, -- Sábado
}

function onLogin(player)
	local dt = os.date('*t')
	if bonusDays[dt.wday] then 
		local nplayers = #Game.getPlayers()
		local changed = false
		for k = #bonusExpStages, 1, -1 do
			local stage = bonusExpStages[k]
			if nplayers >= stage[1] then
				changed = true
				if currentExpBonus < stage[2] then
					Game.broadcastMessage("The server has reached " .. stage[1] .. " players online! The bonus of exp is now " .. ((stage[2] - 1) * 100) .. "%!")
				elseif currentExpBonus > stage[2] then
					Game.broadcastMessage("The exp bonus has been reduced because there aren't enough players online. Current bonus is " .. ((stage[2] - 1) * 100) .. "%!")
				end
				currentExpBonus = stage[2]
				break
			end
		end

		if not changed and currentExpBonus > 1 then
			currentExpBonus = 1
			Game.broadcastMessage("The exp bonus has been deactivated because there aren't enough players online.")
		end
	else
		currentExpBonus = 1
	end

	local loginStr = "Welcome to " .. configManager.getString(configKeys.SERVER_NAME) .. "!"
	if player:getLastLoginSaved() <= 0 then
		loginStr = loginStr .. " Please choose your outfit."
		player:sendOutfitWindow()
	else
		if loginStr ~= "" then
			player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)
		end

		loginStr = string.format("Your last visit was on %s.", os.date("%a %b %d %X %Y", player:getLastLoginSaved()))
	end
	player:sendTextMessage(MESSAGE_STATUS_DEFAULT, loginStr)

	-- Stamina
	nextUseStaminaTime[player.uid] = 0

	-- Promotion
	local vocation = player:getVocation()
	local promotion = vocation:getPromotion()
	if player:isPremium() then
		local value = player:getStorageValue(STORAGEVALUE_PROMOTION)
		if not promotion and value ~= 1 then
			player:setStorageValue(STORAGEVALUE_PROMOTION, 1)
		elseif value == 1 then
			player:setVocation(promotion)
		end
	elseif not promotion then
		player:setVocation(vocation:getDemotion())
	end

	-- Events
	player:registerEvent("PlayerDeath")
	player:registerEvent("DropLoot")
	return true
end
