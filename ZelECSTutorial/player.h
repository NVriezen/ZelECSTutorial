#pragma once
#include <stdint.h> //so we can use uint8_t etc.

typedef struct vector2
{
	float x;
	float y;
};

struct transform_t
{
	vector2 position;
	vector2 rotation;
	vector2 scale;
};

struct rigidbody_t
{
	vector2 velocity;
	vector2 acceleration;
};

struct sprite_t
{
	uint32_t id;
	vector2 size;
};