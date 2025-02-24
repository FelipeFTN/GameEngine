#include "../../include/linmath.h"
#include "physics_internal.h"
#include "array_list.h"
#include "physics.h"
#include "global.h"
#include "types.h"
#include "util.h"

static Physics_State_Internal state;

void aabb_min_max(vec2 min, vec2 max, AABB aabb) {
    vec2_sub(min, aabb.position, aabb.half_size);
    vec2_add(max, aabb.position, aabb.half_size);
}

void physics_init(void) {
    state.body_list = array_list_create(sizeof(Body), 0);
}

void physics_update(void) {
    Body *body;

    for (u32 i = 0; i < state.body_list->len; i++) {
        body = array_list_get(state.body_list, i);
        
        body->velocity[0] += body->acceleration[0] * global.time.delta;
        body->velocity[1] += body->acceleration[1] * global.time.delta;
        body->aabb.position[0] += body->velocity[0] * global.time.delta;
        body->aabb.position[1] += body->velocity[1] * global.time.delta;
    }
}

usize physics_body_create(vec2 position, vec2 size) {
    Body body = {
        .aabb = {
            .position = { position[0], position[1] },
            .half_size = { size[0] * 0.5f, size[1] * 0.5f },
        },
        .velocity = { 0.0f, 0.0f },
    };

    if (array_list_append(state.body_list, &body) == (usize)-1)
        ERROR_EXIT("Could not append body to list\n");

    return state.body_list->len - 1;
}

Body *physics_body_get(usize index) {
    return array_list_get(state.body_list, index);
}

bool physics_point_intersect_aabb(vec2 point, AABB aabb) {
    vec2 min, max;
    aabb_min_max(min, max, aabb);
    return point[0] >= min[0] &&
           point[0] <= max[0] &&
           point[1] >= min[1] &&
           point[1] <= max[1];
}

AABB aabb_minkowski_difference(AABB a, AABB b) {
    AABB result;
    vec2_sub(result.position, a.position, b.position);
    vec2_add(result.half_size, a.half_size, b.half_size);

    return result;
}

bool physics_aabb_intersect_aabb(AABB a, AABB b) {
    vec2 min, max;
    aabb_min_max(min, max, aabb_minkowski_difference(a, b));

    return (min[0] <= 0 && max[0] >= 0) &&
           (min[1] <= 0 && max[1] >= 0);
}

void aabb_penetration_vector(vec2 r, AABB aabb) {
    vec2 min, max;
    aabb_min_max(min, max, aabb);

    f32 min_dist = fabsf(min[0]);
    r[0] = min[0];
    r[1] = 0;

    if (fabsf(max[0]) < min_dist) {
        min_dist = fabsf(max[0]);
        r[0] = 0;
        r[0] = max[0];
    }

    if (fabsf(min[1]) < min_dist) {
        min_dist = fabsf(min[1]);
        r[0] = 0;
        r[1] = min[1];
    }

    if (fabsf(max[1]) < min_dist) {
        r[0] = 0;
        r[1] = max[1];
    }
}

Hit ray_intersect_aabb(vec2 position, vec2 magnitude, AABB aabb) {
	Hit hit = {0};
	vec2 min, max;
	aabb_min_max(min, max, aabb);

	f32 last_entry = -INFINITY;
	f32 first_exit = INFINITY;

	for (u8 i = 0; i < 2; ++i) {
		if (magnitude[i] != 0) {
			f32 t1 = (min[i] - position[i]) / magnitude[i];
			f32 t2 = (max[i] - position[i]) / magnitude[i];

			last_entry = fmaxf(last_entry, fminf(t1, t2));
			first_exit = fminf(first_exit , fmaxf(t1, t2));
		} else if (position[i] <= min[i] || position[i] >= max[i]) {
			return hit;
		}
	}

	if (first_exit > last_entry && first_exit > 0 && first_exit < 1) {
		// Get the position of the hit.
		hit.position[0] = position[0] + magnitude[0] * last_entry;
		hit.position[1] = position[1] + magnitude[1] * last_entry;

		hit.is_hit = true;
		hit.time = last_entry;
	}

	return hit;
}
