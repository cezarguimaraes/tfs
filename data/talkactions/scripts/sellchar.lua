local OFFER_FEE = 20

function onSay(player, words, param)
	local price = tonumber(param)
	if not price then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "usage: " .. words .. " <price>")
		return false
	end

	local account = player:getAccountId()
	local res = db.storeQuery("SELECT `premium_points` FROM `accounts` WHERE `id``= " .. account)
	if not res or result.getNumber(res) < OFFER_FEE then
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "You do not have enough coins to put your character up to sale.")
	end

	if player:addToStore(price) then
		db.query("UPDATE `accounts` SET `premium_points` = `premium_points` - " .. OFFER_FEE .. " WHERE `id` = " .. account)
	else 
		player:sendTextMessage(MESSAGE_STATUS_WARNING, "There are too many character offers up at the moment, please try again later.")
	end
	return false
end