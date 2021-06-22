#include "level.h"

level* level_create()
{
	level* new_level = new level;
	return new_level;
}

void level_destroy(level* level)
{
	std::unordered_map<std::string, ComponentBase*>::iterator level_components_iterator = level->components.begin();
	while (level_components_iterator != level->components.end())
	{
		level_components_iterator->second->free();
		delete level_components_iterator->second;

		++level_components_iterator;
	}

	delete level;
}

void level_register_system(level* level, system_t system_update, const char* system_name)
{
	level->systems.insert({ system_name, system_update });
}

void level_unregister_system(level* level, const char* system_name)
{
	level->systems.erase(system_name);
}

entity_id create_entity(level* level)
{
	if (!level->empty_entities_spots.empty())
	{
		uint32_t empty_spot = level->empty_entities_spots.front();
		level->empty_entities_spots.pop();
		level->entities[empty_spot] += 1;
		entity_id new_entity = CREATE_ID(level->entities[empty_spot], empty_spot);
		return new_entity;
	}

	level->entities.push_back(1);
	return CREATE_ID(1, level->entities.size() - 1);
}

void destroy_entity(level* level, entity_id entity)
{
	uint32_t entity_index = GET_INDEX(entity);
	level->entities[entity_index] += 1;
	level->empty_entities_spots.push(entity_index);

	std::unordered_map<std::string, ComponentBase*>::iterator level_components_iterator = level->components.begin();
	while (level_components_iterator != level->components.end())
	{
		level_components_iterator->second->destroy(entity);

		++level_components_iterator;
	}
}