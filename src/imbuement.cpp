#include "otpch.h"

#include "imbuement.h"

#include "item.h"
#include "pugicast.h"
#include "tools.h"

Imbuement* Imbuements::getImbuement(uint16_t id) {
	auto it = imbuements.find(id);
	if (it == imbuements.end()) {
		std::cout << "[Warning - Imbuements::loadFromXml] Imbuement " << id << " not found" << std::endl;
		return nullptr;
	}
	return &it->second;
}

bool Imbuements::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/imbuements.xml");
	if (!result) {
		printXMLError("Error - Imbuements::loadFromXml", "data/XML/imbuements.xml", result);
		return false;
	}	

	for (auto node : doc.child("imbuements").children()) {
		pugi::xml_attribute attr;

		if (strcasecmp(node.name(), "config") == 0) {
			if (!(attr = node.attribute("prefix"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing prefix for config entry" << std::endl;
				continue;
			}

			std::string prefix = attr.as_string();

			if (config.count(prefix)) {
				std::cout << "[Warning - Imbuements::loadFromXml] Duplicate config prefix " << prefix << std::endl;
				continue;
			}

			uint32_t duration, fee, additional, chance;

			if (!(attr = node.attribute("duration"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing duration for config " << prefix << std::endl;
				continue;
			}

			duration = pugi::cast<uint32_t>(attr.value());

			if (!(attr = node.attribute("fee"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing fee for config " << prefix << std::endl;
				continue;
			}

			fee = pugi::cast<uint32_t>(attr.value());

			if (!(attr = node.attribute("additional"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing additional for config " << prefix << std::endl;
				continue;
			}

			additional = pugi::cast<uint32_t>(attr.value());

			if (!(attr = node.attribute("chance"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing chance for config " << prefix << std::endl;
				continue;
			}

			chance = pugi::cast<uint32_t>(attr.value());

			config[prefix] = std::make_tuple(duration, fee, additional, chance);
		} else if (strcasecmp(node.name(), "group") == 0) {
			if (!(attr = node.attribute("id"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing group id" << std::endl;
				continue;
			}

			uint16_t groupId = pugi::cast<uint16_t>(attr.value());

			if (!(attr = node.attribute("name"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing group name" << std::endl;
				continue;
			}

			auto res = groupName.emplace(groupId, attr.as_string());
			if (!res.second) {
				std::cout << "[Warning - Imbuements::loadFromXml] Duplicate group id " << groupId << std::endl;
				continue;
			}
		} else if (strcasecmp(node.name(), "equipment_type") == 0) {
			if (!(attr = node.attribute("name"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing equipment type name" << std::endl;
				continue;
			}

			const std::string& group = attr.as_string();

			for (auto itemNode : node.children()) {
				if (!(attr = itemNode.attribute("name"))) {
					std::cout << "[Warning - Imbuements::loadFromXml] Missing item name for equipment group " << group << std::endl;
					continue;
				}

				uint16_t itemId = Item::items.getItemIdByName(attr.as_string());				

				if (!(itemId)) {
					std::cout << "[Warning - Imbuements::loadFromXml] Item named '" << attr.as_string() << "' not found" << std::endl;
					continue;
				}

				if (!(attr = itemNode.attribute("slots"))) {
					std::cout << "[Warning - Imbuements::loadFromXml] Missing item slots for equipment group " << group << std::endl;
					continue;
				}

				uint8_t slots = pugi::cast<uint32_t>(attr.value());

				auto res = equipmentType.emplace(itemId, group);
				if (!res.second) {
					std::cout << "[Warning - Imbuements::loadFromXml] Item named '" << attr.as_string() << "' appears in more than one group" << std::endl;
					continue;
				}

				equipmentSlots[itemId] = slots;
			}
		} else if (strcasecmp(node.name(), "imbuement") == 0) {
			if (!(attr = node.attribute("id"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing imbuement id" << std::endl;
				continue;
			}

			uint16_t id = pugi::cast<uint16_t>(attr.value());

			if (!(attr = node.attribute("groupid"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing imbuement group id" << std::endl;
				continue;
			}

			uint16_t groupId = pugi::cast<uint16_t>(attr.value());

			auto res = imbuements.emplace(std::piecewise_construct, 
			                              std::forward_as_tuple(id), 
			                              std::forward_as_tuple(id, groupId));

			if (!res.second) {
				std::cout << "[Warning - Imbuements::loadFromXml] Duplicate imbuement of id '" << id << "' ignored" << std::endl;
				continue;
			}

			Imbuement& imb = res.first->second;

			if (!(attr = node.attribute("name"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing name of imbuement id " << id << std::endl;
				continue;
			}

			imb.name = attr.as_string();

			std::string prefix = imb.name.substr(0, imb.name.find(' '));

			auto it = config.find(prefix);
			if (it == config.end()) {
				std::cout << "[Warning - Imbuements::loadFromXml] Configuration for prefix " << prefix << " not found" << std::endl;
				continue;
			}

			std::tie(imb.duration, imb.fee, imb.additional, imb.chance) = it->second;

			if (!(attr = node.attribute("description"))) {
				std::cout << "[Warning - Imbuements::loadFromXml] Missing description of imbuement id " << id << std::endl;
				continue;
			}

			imb.description = attr.as_string();

			if ((attr = node.attribute("storage"))) {
				imb.storage = pugi::cast<int32_t>(attr.value());
			}

			for (auto childNode : node.children()) {
				if (strcasecmp(childNode.name(), "equipment") == 0) {
					if (!(attr = childNode.attribute("type"))) {
						std::cout << "[Warning - Imbuements::loadFromXml] Missing equipment type name for imbuement id " << id << std::endl;
						continue;
					}

					imb.equipmentTypes.emplace(attr.as_string());
				} else if (strcasecmp(childNode.name(), "source") == 0) {
					if (!(attr = childNode.attribute("name"))) {
						std::cout << "[Warning - Imbuements::loadFromXml] Missing astral source name for imbuement id " << id << std::endl;
						continue;
					}

					uint16_t sourceId = Item::items.getItemIdByName(attr.as_string());

					if (!(sourceId)) {
						std::cout << "[Warning - Imbuements::loadFromXml] Item named '" << attr.as_string() << "' not found" << std::endl;
						continue;
					}

					if (!(attr = childNode.attribute("count"))) {
						std::cout << "[Warning - Imbuements::loadFromXml] Missing astral source count for imbuement id " << id << std::endl;
						continue;
					}

					uint16_t count = pugi::cast<uint16_t>(attr.value());

					if (imb.astralSources.count(sourceId)) {
						std::cout << "[Warning - Imbuements::loadFromXml] Duplicate astral source for imbuement id " << id << " ignored" << std::endl;
						continue;
					}

					imb.astralSources[sourceId] = count;
				} else if (strcasecmp(childNode.name(), "effect") == 0) {
					if (!(attr = childNode.attribute("type"))) {
						std::cout << "[Warning - Imbuements::loadFromXml] Missing effect type for imbuement id " << id << std::endl;
						continue;
					}

					std::string type = attr.as_string();

					if (strcasecmp(type.c_str(), "skill") == 0) {
						if (!(attr = childNode.attribute("name"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing skill name for imbuement id " << id << std::endl;
							continue;
						}

						std::string name = attr.as_string();
						uint8_t skillId;

						if (strcasecmp(name.c_str(), "sword") == 0) {
							skillId = SKILL_SWORD;
						} else if (strcasecmp(name.c_str(), "axe") == 0) {
							skillId = SKILL_AXE;
						} else if (strcasecmp(name.c_str(), "club") == 0) {
							skillId = SKILL_CLUB;
						} else if (strcasecmp(name.c_str(), "distance") == 0) {
							skillId = SKILL_DISTANCE;
						} else if (strcasecmp(name.c_str(), "shielding") == 0) {
							skillId = SKILL_SHIELD;
						}

						if (!(attr = childNode.attribute("bonus"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing skill bonus for imbuement id " << id << std::endl;
							continue;
						}

						int32_t bonus = pugi::cast<int32_t>(attr.value());

						imb.skills[skillId] = bonus;
					} else if (strcasecmp(type.c_str(), "ml") == 0) {
						if (!(attr = childNode.attribute("bonus"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing magic level bonus for imbuement id " << id << std::endl;
							continue;
						}

						int32_t bonus = pugi::cast<int32_t>(attr.value());

						imb.stats[STAT_MAGICPOINTS] = bonus;
					} else if (strcasecmp(type.c_str(), "speed") == 0) {
						if (!(attr = childNode.attribute("bonus"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing speed bonus for imbuement id " << id << std::endl;
							continue;
						}

						int32_t bonus = pugi::cast<int32_t>(attr.value());

						imb.speed = bonus;
					} else if (strcasecmp(type.c_str(), "damage_reduction") == 0) {
						if (!(attr = childNode.attribute("percent"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing damage reduction percentage for imbuement id " << id << std::endl;
							continue;
						}

						int32_t percent = pugi::cast<int32_t>(attr.value());

						if (!(attr = childNode.attribute("element"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing damage reduction element for imbuement id " << id << std::endl;
							continue;
						}

						CombatType_t combatType = getCombatType(attr.as_string());

						if (combatType == COMBAT_NONE) {
							std::cout << "[Warning - Imbuements::loadFromXml] Unknown combat type for element" << attr.as_string() << std::endl;
							continue;
						}

						imb.absorbPercent[combatTypeToIndex(combatType)] = percent;
					} else if (strcasecmp(type.c_str(), "crit") == 0) {
						if (!(attr = childNode.attribute("percent"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing crit chance for imbuement id " << id << std::endl;
							continue;
						}

						int32_t percent = pugi::cast<int32_t>(attr.value());

						if (!(attr = childNode.attribute("chance"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing crit chance for imbuement id " << id << std::endl;
							continue;
						}

						int32_t chance = std::min<int32_t>(100, pugi::cast<int32_t>(attr.value()));

						imb.critBonus = percent;
						imb.critChance = chance;
					} else if (strcasecmp(type.c_str(), "mana_leech") == 0) {
						if (!(attr = childNode.attribute("percent"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing mana leech percentage for imbuement id " << id << std::endl;
							continue;
						}

						int32_t percent = pugi::cast<int32_t>(attr.value());

						if (!(attr = childNode.attribute("chance"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing mana leech chance for imbuement id " << id << std::endl;
							continue;
						}

						int32_t chance = pugi::cast<int32_t>(attr.value());

						imb.manaLeechPercent = percent;
						imb.manaLeechChance = chance;
					} else if (strcasecmp(type.c_str(), "life_leech") == 0) {
						if (!(attr = childNode.attribute("percent"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing life leech percentage for imbuement id " << id << std::endl;
							continue;
						}

						int32_t percent = pugi::cast<int32_t>(attr.value());

						if (!(attr = childNode.attribute("chance"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing life leech chance for imbuement id " << id << std::endl;
							continue;
						}

						int32_t chance = pugi::cast<int32_t>(attr.value());

						imb.lifeLeechPercent = percent;
						imb.lifeLeechChance = chance;
					} else if (strcasecmp(type.c_str(), "element_damage") == 0) {
						if (!(attr = childNode.attribute("percent"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing element damage percentage for imbuement id " << id << std::endl;
							continue;
						}

						int32_t percent = pugi::cast<int32_t>(attr.value());

						if (!(attr = childNode.attribute("element"))) {
							std::cout << "[Warning - Imbuements::loadFromXml] Missing element for imbuement id " << id << std::endl;
							continue;
						}

						CombatType_t combatType = getCombatType(attr.as_string());

						if (combatType == COMBAT_NONE) {
							std::cout << "[Warning - Imbuements::loadFromXml] Unknown combat type for element" << attr.as_string() << std::endl;
							continue;
						}

						imb.absorbPercent[combatTypeToIndex(combatType)] = percent;
					}
				}
			}

			printf("name: %s, category: %s\n", imb.getName().c_str(), groupName[imb.groupId].c_str());
		}
	}

	return true;
}

Imbuement::Imbuement(uint16_t id, uint16_t groupId)
	: stats(), skills(), absorbPercent(), elementDamagePercent(), id(id), groupId(groupId)
{
	speed = critBonus = critChance = 0;
	manaLeechPercent = manaLeechChance = 0;
	lifeLeechPercent = lifeLeechChance = 0;
	storage = -1;
	duration = fee = additional = chance = 0;
}

