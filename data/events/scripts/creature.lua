function Creature:onChangeOutfit(outfit)
	if self:isPlayer() then
		local battlefieldInfo = Battlefield:findPlayer(self)
		if battlefieldInfo then
			return false
		end
	end
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return true
end

function Creature:onTargetCombat(target)
	return true
end
