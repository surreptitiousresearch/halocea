/* actor_move_initialize @ 0x837C8488 — precompute the actor sensing/avoidance ray
 * tables (directions, offsets, divergences) from fixed angle/divergence constants. */

#include "headers/real_vector3d.h"
#include "headers/sense_ray.h"

#include <math.h>
#include "headers/blam_data_globals.h"

extern float sense_ray_angles[9];
extern float sense_ray_divergences[9];
extern float sense_ray_offsets[9];

extern float avoidance_ray_divergence[2];
extern float avoidance_ray_angles[8];
extern float avoidance_ray_offsets[2];

void actor_move_initialize(void)
{
    int v;
    int a;

    for ( v = 0; v < 9; ++v )
    {
        float s = (float)sin(sense_ray_angles[v]);
        float c = (float)cos(sense_ray_angles[v]);
        float ds = (float)sin((sense_ray_divergences[v] * 0.052359879f));  /* deg->rad */
        float dc = (float)cos((sense_ray_divergences[v] * 0.052359879f));
        float off_c = (sense_ray_offsets[v] * c);
        float off_s = (sense_ray_offsets[v] * s);

        sense_rays[v].length = 1.0f;
        sense_rays[v].offset.n[0] = 0.0f;
        sense_rays[v].offset.n[1] = (off_c * 0.69999999f);
        sense_rays[v].offset.n[2] = (off_s * 0.69999999f);
        sense_rays[v].divergence.n[0] = dc;
        sense_rays[v].divergence.n[1] = (ds * c);
        sense_rays[v].divergence.n[2] = (ds * s);
    }

    for ( a = 0; a < 2; ++a )
    {
        float adiv_s = (float)sin(avoidance_ray_divergence[a]);
        float adiv_c = (float)cos(avoidance_ray_divergence[a]);
        int k;
        for ( k = 0; k < 8; ++k )
        {
            float ang_s = (float)sin(avoidance_ray_angles[k]);
            float ang_c = (float)cos(avoidance_ray_angles[k]);
            float off = avoidance_ray_offsets[a];
            /* DEVIATION: the flat slot 2*k+a was folded through avoidance_rays[0]; the array is [8][2], so [k][a] is the same address. */

            avoidance_directions[k].n[0] = 0.0f;
            avoidance_directions[k].n[1] = ang_c;
            avoidance_directions[k].n[2] = ang_s;

            avoidance_rays[k][a].length = 0.69999999f;
            avoidance_rays[k][a].offset.n[0] = (0.0f * off);
            avoidance_rays[k][a].offset.n[1] = (avoidance_directions[k].n[1] * off);
            avoidance_rays[k][a].offset.n[2] = (avoidance_directions[k].n[2] * off);
            avoidance_rays[k][a].divergence.n[0] = adiv_c;
            avoidance_rays[k][a].divergence.n[1] = (avoidance_directions[k].n[1] * adiv_s);
            avoidance_rays[k][a].divergence.n[2] = (avoidance_directions[k].n[2] * adiv_s);
        }
    }
}
