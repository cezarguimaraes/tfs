#ifndef FS_IMBUEMENT_H
#define FS_IMBUEMENT_H

#include "enums.h"

#include <set>

class Imbuement
{
	public:
		Imbuement(uint16_t id, uint16_t groupId);

		uint16_t getId() const {
			return id;
		}

		uint16_t getGroupId() const {
			return groupId;
		}

		const std::string& getName() const {
			return name;
		}

		const std::string& getDescription() const {
			return description;
		}

		int32_t stats[STAT_LAST + 1];

		int32_t skills[SKILL_LAST + 1];

		int32_t speed;

		int32_t critBonus, critChance;

		int32_t manaLeechPercent, manaLeechChance;
		int32_t lifeLeechPercent, lifeLeechChance;

		int16_t absorbPercent[COMBAT_COUNT];
		int16_t elementDamagePercent[COMBAT_COUNT];

	protected:
		friend class Imbuements;

		std::string name;
		std::string description;

		uint16_t id, groupId;

		int32_t storage;

		uint32_t duration, fee, additional, chance;

		std::set<std::string> equipmentTypes;
		std::unordered_map<uint16_t, uint16_t> astralSources;
};

class Imbuements
{
	public:
		bool loadFromXml();

		Imbuement* getImbuement(uint16_t id);

	protected:
		friend class Item;

		std::unordered_map<uint16_t, uint8_t> equipmentSlots;

	private:
		std::unordered_map<uint16_t, Imbuement> imbuements;
		std::unordered_map<uint16_t, std::string> equipmentType;
		std::unordered_map<uint16_t, std::string> groupName;

		std::unordered_map<std::string, std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>> config;
};

#endif
