#include <stdlib.h>

#include "vec2d.h"


// Constructor
Vec2D *new_vec2d(int x, int y) {
	Vec2D *vec = malloc(sizeof(Vec2D));
	vec->x = x;
	vec->y = y;
	return vec;
}

// Checks if two Vec2Ds are equal
int vec2d_equals(const Vec2D a, const Vec2D b) {
	return a.x == b.x && a.y == b.y;
}

// Set values for Vec2D
void set_vec2d(Vec2D* vec, int x, int y) {
	vec->x = x;
	vec->y = y;
}
