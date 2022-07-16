#include "circle.h"

Circle::Circle(Color Color, float Radius, float Mass, V2 Velocity, V2 Position) {
	color = Color;
	radius = Radius;
	mass = Mass;
	velocity = Velocity;
	position = Position;
}

Circle::Circle(Color Color, float Radius, V2 Velocity, V2 Position) {
	color = Color;
	radius = Radius;
	mass = pow(radius, 2);
	velocity = Velocity;
	position = Position;
}

Circle::Circle(float Radius, V2 Position) {
	color = RED;
	radius = Radius;
	mass = pow(radius, 2);
	velocity = V2();
	position = Position;
}