#include <stdint.h>
#include "vec.h"

struct vec vec_mul(struct vec vec, int scalar) {
	return (struct vec){vec.x * scalar, vec.y * scalar};
}

struct vec vec_add(struct vec vec1, struct vec vec2)
{
	return (struct vec){ vec1.x + vec2.x, vec1.y + vec2.y };
}

struct vec vec_sub(struct vec vec1, struct vec vec2) {
	return (struct vec){ vec1.x - vec2.x, vec1.y - vec2.y };
}

bool vec_eq(struct vec vec1, struct vec vec2)
{
	return vec1.x == vec2.x && vec1.y == vec2.y;
}

bool vec_in_bounds(struct vec vec, uint32_t max_x, uint32_t max_y)
{
	return vec.x >= max_x || vec.y >= max_y;
}
