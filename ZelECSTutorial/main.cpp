#include <stdio.h>
#include "movement_system.h"
#include "entities.h"
#include "level.h"

void main()
{
	level* test_level = level_create();
	entity_id player_entity = create_entity(test_level);

	level_register_component<transform_t>(test_level);
	level_register_component<rigidbody_t>(test_level);

	transform_t transform;
	transform.position.x = 0;
	transform.position.y = 0;

	rigidbody_t rigidbody;
	rigidbody.acceleration.x = 1.0f;
	rigidbody.acceleration.y = 0;
	rigidbody.velocity.x = 0;
	rigidbody.velocity.y = 0;

	level_add_component<transform_t>(test_level, player_entity, transform);
	level_add_component<rigidbody_t>(test_level, player_entity, rigidbody);

	level_register_system(test_level, movement_update, movement_name);

	//We also hard code the delta time for now
	//We aim for 60FPS, so one frame should take
	//1/60th of a second
	float delta_time = 1.0f / 60.0f;

	while (1)
	{
		for (std::pair<std::string, system_t> system : test_level->systems)
		{
			system.second(test_level, delta_time);
		}

		transform_t* player_transform = level_get_component<transform_t>(test_level, player_entity);
		printf("Player's position [%d]: %0.2f %0.2f\n", 
			player_entity, 
			player_transform->position.x, 
			player_transform->position.y
		);
	}

	destroy_entity(test_level, player_entity);
	level_destroy(test_level);
}