#pragma once
/* rasterizer_global_defaults — default clip-plane distances applied when none are set (DB layout, 16 bytes).
 * Uses a struct tag (not a typedef) so the global may legally share the name rasterizer_global_defaults. */

struct rasterizer_global_defaults
{
    float z_near;              /* 0x00 */
    float z_far;               /* 0x04 */
    float z_near_first_person; /* 0x08 */
    float z_far_first_person;  /* 0x0C */
};

extern struct rasterizer_global_defaults rasterizer_global_defaults;
