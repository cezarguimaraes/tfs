function onRender(player, offer)
	return true
end

function onBuy(player, offer)
	local itemType = ItemType(offer:getName())
	if itemType:getId() == 0 then
		player:sendStoreError(STORE_ERROR_PURCHASE, "Item not found. Please contact an administrator.")
		return false
	end

	player:addItem(itemType:getId(), 1)
	return true
end