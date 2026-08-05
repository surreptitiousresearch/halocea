#include <stdint.h>

#pragma once
/* euler_axis_plan — a bang-bang (accelerate/coast/decelerate) motion plan for one rotational axis.
 * Layout verbatim from the database. */

typedef struct euler_axis_plan
{
    uint8_t stopped;   /* 0x00 — plan already at rest / not running */
    char            _pad1[3];  /* 0x01 */
    float           initial_p; /* 0x04 — starting position */
    float           initial_v; /* 0x08 — starting velocity */
    float           accel_a;   /* 0x0C — acceleration magnitude */
    float           accel_t;   /* 0x10 — remaining time in the acceleration phase */
    float           coast_t;   /* 0x14 — remaining time in the coast phase */
    float           decel_a;   /* 0x18 — deceleration magnitude */
    float           decel_t;   /* 0x1C — remaining time in the deceleration phase */
} euler_axis_plan;

extern void unit_euler_axis_buildplan(float current_location, float current_velocity, float velocity_limit, float acceleration_limit, euler_axis_plan *plan);
extern void unit_euler_axis_couple(euler_axis_plan *plan1, euler_axis_plan *plan2, float velocity_limit, float acceleration_limit);
extern uint8_t unit_euler_axis_doplan(euler_axis_plan *plan, float delta_t, float current_location, float *out_location, float current_velocity, float *out_velocity);
