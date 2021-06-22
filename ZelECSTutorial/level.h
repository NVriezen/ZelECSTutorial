#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <typeinfo>
#include "component_base.h"
#include "component_class.h"

typedef struct level _level;
typedef void(*system_t)(_level* level, float delta_time);

struct level
{
	//components
	std::unordered_map<std::string, ComponentBase*> components;

	//systems
	std::unordered_map<std::string, system_t> systems;

	//entities
	std::vector<uint8_t> entities = { 0 };
	std::queue<uint32_t> empty_entities_spots;
};

level* level_create();
void level_destroy(level* level);
void level_register_system(level* level, system_t system_update, const char* system_name);
void level_unregister_system(level* level, const char* system_name);
entity_id create_entity(level* level);
void destroy_entity(level* level, entity_id entity);

template <typename T>
void level_register_component(level* level)
{
	std::string type_name = typeid(T).name();

	if (level->components.find(type_name) != level->components.end())
	{
		//The component type is already registered
		return;
	}

	Component<T>* new_component_type = new Component<T>();
	ComponentBase* base_component_type = new_component_type;
	level->components.insert({ type_name, base_component_type });
}

template <typename T>
void level_register_component_with_destroy(level* level, void(*destroy_function)(T*))
{
	std::string type_name = typeid(T).name();

	if (level->components.find(type_name) != level->components.end())
	{
		//The component type is already registered
		return;
	}

	Component<T>* new_component_type = new Component<T>();
	new_component_type->destroy_function = destroy_function;
	ComponentBase* base_component_type = new_component_type;
	level->components.insert({ type_name, base_component_type });
}

template <typename T>
void level_add_component(level* level, entity_id entity, T component)
{
	std::string type_name = std::string(typeid(T).name());
	Component<T>* component_type = (Component<T>*)(level->components[type_name]);

	component_type->create(entity, component);
}

template <typename T>
T* level_get_component(level* level, entity_id entity)
{
	std::string type_name = std::string(typeid(T).name());
	Component<T>* component_type = (Component<T>*)(level->components[type_name]);

	return component_type->get_component(entity);
}

template <typename... T>
bool level_has_components(level* level, entity_id entity)
{
	std::string component_names[] = { "", std::string(typeid(T).name())... };

	uint32_t types_size = sizeof...(T);
	std::vector<ComponentBase*> type_bases;
	for (size_t i = 1; i < types_size + 1; i++)
	{
		type_bases.push_back(level->components[component_names[i]]);
	}

	for (size_t j = 0; j < types_size; j++)
	{
		if (!type_bases[j]->has_component(entity))
			return false;
	}

	return true;
}