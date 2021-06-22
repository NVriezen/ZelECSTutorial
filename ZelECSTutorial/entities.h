#pragma once
#include <stdint.h>

#define MAX_ENTITIES 0xFFFF

#define CAPACITY_ENTITIES 0xFFFFFF
#define GENERATION_BIT 24

#define GET_GENERATION(id) ((id & 0xFF000000) >> GENERATION_BIT)
#define GET_INDEX(id) (id & CAPACITY_ENTITIES)
#define CREATE_ID(generation, index) (generation << GENERATION_BIT) | index

// The upper 8 bits is the generation
// The lower 24 bits is the actual index in the array
typedef uint32_t entity_id;

struct generational_ptr
{
	entity_id id;
	uint8_t generation;
};