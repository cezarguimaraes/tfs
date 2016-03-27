function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	if param == 'open' then
		Battlefield:Open()
	elseif param == 'close' then
		Battlefield:Close()
	elseif param == 'join' then
		Battlefield:onJoin(player)
	elseif param == 'leave' then
		Battlefield:onLeave(player)
	elseif param == 'death' then
		Battlefield:onDeath(player)
	elseif paran == 'kick' then
		player:remove()
	elseif param == 'tp' then
		local tp = Game.createItem(1387, 1, player:getPosition())
		tp:setActionId(4040)
	end

	return false
end
