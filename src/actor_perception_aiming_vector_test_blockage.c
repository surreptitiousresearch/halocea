/* actor_perception_aiming_vector_test_blockage @0x837D7DF4 — test whether a friendly unit lies along an
 * actor's aiming vector such that firing would risk hitting it. Projects the source-to-friend offset onto
 * the (horizontally normalized) aiming direction; only friends roughly ahead (within ~30 degrees) are
 * considered. The perpendicular offset from the aim line classifies the blockage: 2 = directly blocking,
 * 1 = marginal vertical band, 0 = clear, with a final horizontal-distance gate. Optionally outputs the
 * (negated) perpendicular offset vector.
 *
 * DEVIATION: the decompiler renders source_vector->n[0] as COERCE_FLOAT("nit") in the magnitude/projection
 * expressions; both are the same incoming float, named source_vector->n[0] here per its later use. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern double __fabs(double x);

int16_t actor_perception_aiming_vector_test_blockage(const real_point3d *source_position,
                                                     const real_vector3d *source_vector,
                                                     const real_point3d *friend_position,
                                                     real_vector3d *friend_direction_to_aiming_vector)
{
    int16_t result = 0;

    float horizontal_magnitude =
        __fsqrts((source_vector->n[0] * source_vector->n[0]) +
                 (source_vector->n[1] * source_vector->n[1]));

    if (__fabs(horizontal_magnitude) >= 0.000099999997f && horizontal_magnitude > 0.0f)
    {
        float delta_x = friend_position->n[0] - source_position->n[0];
        float delta_y = friend_position->n[1] - source_position->n[1];
        float inverse_magnitude = 1.0f / horizontal_magnitude;

        float forward_distance =
            (delta_x * (inverse_magnitude * source_vector->n[0])) +
            (delta_y * (inverse_magnitude * source_vector->n[1]));

        float horizontal_distance = __fsqrts((delta_y * delta_y) + (delta_x * delta_x));

        if (forward_distance > (horizontal_distance * 0.86602539f))   /* cos(30 degrees) */
        {
            float perpendicular_x = (source_vector->n[0] * -forward_distance) +
                                    (friend_position->n[0] - source_position->n[0]);
            float perpendicular_y = (source_vector->n[1] * -forward_distance) +
                                    (friend_position->n[1] - source_position->n[1]);
            float perpendicular_z = (source_vector->n[2] * -forward_distance) +
                                    (friend_position->n[2] - source_position->n[2]);

            if (friend_direction_to_aiming_vector)
            {
                friend_direction_to_aiming_vector->n[0] = -perpendicular_x;
                friend_direction_to_aiming_vector->n[1] = -perpendicular_y;
                friend_direction_to_aiming_vector->n[2] = -perpendicular_z;
            }

            if (perpendicular_z <= -0.5f || perpendicular_z >= 0.89999998f)
            {
                if (perpendicular_z <= -0.80000001f || (result = 1, perpendicular_z >= 1.2f))
                    result = 0;
            }
            else
            {
                result = 2;
            }

            if (result > 0)
            {
                float perpendicular_horizontal_sq =
                    (perpendicular_y * perpendicular_y) + (perpendicular_x * perpendicular_x);
                if (perpendicular_horizontal_sq >= 0.36000001f)
                    return perpendicular_horizontal_sq < 1.21f;
            }
        }
    }
    return result;
}
