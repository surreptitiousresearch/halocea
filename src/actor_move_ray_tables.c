/* Flight vector-avoidance ray tables — recovered from the binary's data segment (big-endian floats):
 * sense_ray_divergence @0x821265C0, sense_ray_offsets @0x821265C4, sense_ray_divergences @0x821265E8,
 * sense_ray_angles @0x8212660C, avoidance_ray_angles @0x82126634, sense_ray_avoidance_weights @0x82126668.
 * These were previously extern-only ("fan geometry unrecoverable"); values below are the shipped constants.
 * Geometry: sense ray 0 is dead ahead; rays 1-4 are an inner ring (offset 0.5, divergence 0.3) and rays 5-8
 * an outer ring (offset 1.0, divergence 1.0), both at 0/90/180/270 degrees. Escape ring is a 45-degree-step
 * clock. Weight rows 0-4 are zero (forward + inner ring do not steer); each outer-ray row is the kernel
 * [1, .7, 0, -.5, -.5, -.5, 0, .7] rotated so the +1 sits on the escape direction opposite the blocked ray. */

#include "headers/math_constants.h"

float sense_ray_divergence = 0.052359879f; /* pi/60 */

float sense_ray_offsets[9] =
    { 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f };

float sense_ray_divergences[9] =
    { 0.0f, 0.3f, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f, 1.0f, 1.0f };

float sense_ray_angles[9] = /* radians: 0, 0/90/180/270 (inner), 0/90/180/270 (outer) */
    { 0.0f, 0.0f, 1.5707964f, PI, 4.712389f, 0.0f, 1.5707964f, PI, 4.712389f };

float avoidance_ray_angles[8] = /* radians: 45-degree steps */
    { 0.0f, 0.78539819f, 1.5707964f, 2.3561945f, PI, 3.9269909f, 4.712389f, 5.4977871f };

float sense_ray_avoidance_weights[9][8] =
{
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -0.5f, -0.5f, 0.0f, 0.7f, 1.0f, 0.7f, 0.0f, -0.5f },
    { 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.7f, 1.0f, 0.7f },
    { 1.0f, 0.7f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.7f },
    { 0.0f, 0.7f, 1.0f, 0.7f, 0.0f, -0.5f, -0.5f, -0.5f },
};

/* Escape-ring probe weights, one per side of each direction's ray pair (avoidance_rays[dir][0/1]).
 * Recovered @0x82126788; the second (offset) ray of the pair counts 1.5x the first. */
float avoid_ray_avoidance_weights[2] = { 0.8f, 1.2f };

/* Escape-ring pair geometry: each direction's two probe rays sit at these offsets along the
 * direction (x0.7 scaled in code) with these divergences (30 and 55 degrees).
 * Recovered @0x82126654 / @0x8212665C. */
float avoidance_ray_offsets[2]    = { 0.7f, 1.0f };
float avoidance_ray_divergence[2] = { 0.52359903f, 0.95993108f };
