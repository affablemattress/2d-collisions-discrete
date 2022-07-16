#pragma once
#include "include/raylib.h"
#include "V2.h"

struct Circle
{
	Color color;
	float radius;
	float mass;
	V2 velocity;
	V2 position;
	Circle(Color Color = RED, float Radius = 1, float Mass = 1, V2 Velocity = V2(), V2 Position = V2());
	Circle(Color Color = RED, float Radius = 1, V2 Velocity = V2(), V2 Position = V2());
	Circle(float Radius = 1, V2 Position = V2());
};