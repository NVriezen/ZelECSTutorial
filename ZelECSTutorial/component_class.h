#pragma once
#include "component_base.h"

template <typename T>
class Component : public ComponentBase
{
public:
	T* create(entity_id entity, T component)
	{
		if (last_component == MAX_ENTITIES - 1)
		{
			printf("Maximum amount reached, can't add any more components\n");
			return nullptr;
		}

		++last_component;

		uint32_t last_index = last_component;
		components.push_back(component);
		component_to_entity[last_index].id = entity;
		component_to_entity[last_index].generation += 1;
		uint32_t new_component_id = CREATE_ID(component_to_entity[last_index].generation, last_component);

		uint32_t entity_index = GET_INDEX(entity);
		uint8_t entity_generation = GET_GENERATION(entity);
		entity_to_component[entity_index].id = new_component_id;
		entity_to_component[entity_index].generation = entity_generation;

		return &components[last_index];
	}

	T* get_component(entity_id entity)
	{
		uint32_t entity_index = GET_INDEX(entity);
		uint8_t entity_generation = GET_GENERATION(entity);
		generational_ptr component_pointer = entity_to_component[entity_index];
		if (component_pointer.generation != entity_generation)
		{
			printf("[zel_component class] get component return nullptr | Entity: %d | Type: %s\n", entity, typeid(T).name());
			return nullptr;
		}

		uint32_t component_index = GET_INDEX(component_pointer.id);
		return &components[component_index];
	}

	void destroy(entity_id entity)
	{
		uint32_t entity_to_destroy_index = GET_INDEX(entity);
		generational_ptr component_pointer = entity_to_component[entity_to_destroy_index];
		if (component_pointer.generation != GET_GENERATION(entity))
		{
			printf("Generation not equal, component already destroyed.\n");
			return;
		}

		uint32_t component_to_destroy_index = GET_INDEX(component_pointer.id);

		destroy_function(&components[component_to_destroy_index]);

		if (component_to_destroy_index == last_component)
		{
			component_to_entity[component_to_destroy_index].id = 0;
			component_to_entity[component_to_destroy_index].generation += 1;
			entity_to_component[entity_to_destroy_index].id = 0;
			entity_to_component[entity_to_destroy_index].generation += 1;
			return;
		}

		// Fill in empty spot in array
		generational_ptr entity_pointer_to_swap = component_to_entity[last_component];
		components[component_to_destroy_index] = components[last_component];
		component_to_entity[component_to_destroy_index] = entity_pointer_to_swap;
		component_to_entity[last_component].generation += 1;

		entity_to_component[GET_INDEX(entity_pointer_to_swap.id)].id = component_pointer.id;
		entity_to_component[entity_to_destroy_index].generation += 1;
	}

	void free()
	{
		if (destroy_function == NULL)
		{
			components.clear();
			return;
		}

		uint32_t components_size = components.size();
		for (size_t i = 1; i < components_size; ++i)
		{
			destroy_function(&components[i]);
		}
		components.clear();
	}

	void(*destroy_function)(T*);

private:
	std::vector<T> components = { {} };
};