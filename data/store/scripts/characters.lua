function onRender(player, offer)
	return true
end

function onBuy(player, offer)
	local name = offer:getName()
	local pid, accid = offer:getInfo()
	offer:setAvailable(false)
	
	db.query("DELETE FROM `store_characters` WHERE `player_id` = " .. pid)
	db.query("UPDATE `players` SET `account_id` = " .. player:getAccountId() .. " WHERE `id` = " .. pid)
	db.query("UPDATE `accounts` SET `premium_points` = `premium_points` + " .. offer:getPrice() .. " WHERE `id` = " .. accid)
	return true
end