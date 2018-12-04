#ifndef VEC2D_H
#define VEC2D_H

typedef struct {
	int x;
	int y;
} Vec2D;


// Constructor for Vec2D
Vec2D *new_vec2d(int x, int y);

// Checks if two Vec2Ds are equal
int vec2d_equals(const Vec2D a, const Vec2D b);

// Set values for Vec2D
void set_vec2d(Vec2D* vec, int x, int y);

#endif // VEC2D_H
