#pragma once
#include "player.h"
#include "level.h"

const char* movement_name = "movement";

void movement_update(level* level, float delta_time)
{
	std::vector<uint8_t>::iterator entities_iterator = level->entities.begin();
	std::vector<uint8_t>::iterator entities_begin = level->entities.begin();
	while (entities_iterator != level->entities.end())
	{
		uint32_t entity_index = entities_iterator - entities_begin;
		entity_id entity = CREATE_ID((*entities_iterator), entity_index);

		if (entity != 0 && level_has_components<transform_t, rigidbody_t>(level, entity))
		{
			transform_t* entity_transform = level_get_component<transform_t>(level, entity);
			rigidbody_t* entity_rigidbody = level_get_component<rigidbody_t>(level, entity);

			entity_rigidbody->velocity.x = entity_rigidbody->velocity.x + (entity_rigidbody->acceleration.x * delta_time);
			entity_rigidbody->velocity.y = entity_rigidbody->velocity.y + (entity_rigidbody->acceleration.y * delta_time);
			entity_transform->position.x += entity_rigidbody->velocity.x * delta_time;
			entity_transform->position.y += entity_rigidbody->velocity.y * delta_time;
		}
		entities_iterator++;
	}
}