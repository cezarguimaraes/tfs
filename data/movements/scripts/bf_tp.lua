function onStepIn(creature, item, position, fromPosition)
	if not Battlefield:onJoin(creature) then
		creature:teleportTo(fromPosition)
		return false
	end
	return true
end
