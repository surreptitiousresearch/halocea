/* wind_variance_get @0x837C1E78 — sample a smoothly varying wind gust vector at a world position. For
 * each axis it derives an index into the precomputed wind variance noise table from the position and the
 * global wind time, accumulates the three table vectors, and scales the result by a third of the maximum
 * magnitude.
 *
 * NOTE: the decompiler modeled the output pointer as a packed __int64 (`wind`); reconstructed here as the
 * real `real_vector3d *out` parameter. */

#include <stdint.h>
#include <string.h>
#include "headers/wind_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern double fabs(double x);


void wind_variance_get(const real_point3d *position, real_vector3d *out, float wind_local_variation_rate,
                       float max_magnitude)
{
    static const float axis_scale[3] = { 0.1f, 0.2f, 0.07f };
    float magnitude = (max_magnitude * 0.33333334f);
    int axis;

    *out = *global_zero_vector3d;

    for ( axis = 0; axis < 3; ++axis )
    {
        float time = (float)wind_globals.time;
        /* DEVIATION: the index is the low 6 mantissa bits of the bias-added float read as an integer word
         * (stfs f6 @0x837C1F64; lwz r9 @0x837C1F68; clrlwi r5,r9,26 @0x837C1F6C), the +8388608.0 magic-bias
         * trick — not a float->int convert (rounds-to-nearest, where a convert truncates). */
        float key = fabs((((axis_scale[axis] * time) * wind_local_variation_rate)
                                 + position->n[axis]) * 8.0f) + 8388608.0f;
        uint32_t key_bits;
        memcpy(&key_bits, &key, sizeof(key_bits));
        int index = (axis << 6) + (int16_t)(key_bits & 0x3F);
        const real_vector3d *sample = &wind_globals.variance[0][index];

        out->n[0] = sample->n[0] + out->n[0];
        out->n[1] = sample->n[1] + out->n[1];
        out->n[2] = sample->n[2] + out->n[2];
    }

    out->n[0] = out->n[0] * magnitude;
    out->n[1] = out->n[1] * magnitude;
    out->n[2] = out->n[2] * magnitude;
}
