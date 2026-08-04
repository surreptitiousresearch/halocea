/* rasterizer_frame_begin @0x836A5D28 — apply default near/far clip planes (world and first-person) if they
 * haven't been set, then forward to the platform rasterizer frame-begin.
 * DEVIATION: the tail call rendered as self-recursion is actually _rasterizer_frame_begin (underscore alias). */

#include "headers/rasterizer_globals.h"

typedef struct rasterizer_frame_begin_parameters rasterizer_frame_begin_parameters;

extern void _rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters);

void rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters)
{
    if ( rasterizer_globals.z_near == 0.0f )
        rasterizer_globals.z_near = 0.0625f;
    if ( rasterizer_globals.z_far == 0.0f )
        rasterizer_globals.z_far = 1024.0f;
    if ( rasterizer_globals.z_near_first_person == 0.0f )
        rasterizer_globals.z_near_first_person = 0.01171875f;
    if ( rasterizer_globals.z_far_first_person == 0.0f )
        rasterizer_globals.z_far_first_person = 1024.0f;
    _rasterizer_frame_begin(parameters);
}
