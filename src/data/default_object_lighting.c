/* default_object_lighting @ 0x821140C8 — .rdata, 116 bytes = exactly sizeof(render_lighting).
 * The fallback lighting environment lights_distant_lighting_at_point memcpy's into an object's
 * render_lighting when the structure BSP carries no default lighting of its own.
 * Initializer reconstructed from the binary (big-endian words), field by field:
 *   +0x00 ambient_color          0.2, 0.2, 0.2                (0x3E4CCCCD x3)
 *   +0x0C distant_light_count    2                            (hi16 of 0x00020000)
 *   +0x0E pad                    0
 *   +0x10 distant_lights[0]      color 1,1,1 / direction -0.577,-0.577,-0.577 (0xBF13B646 x3)
 *   +0x28 distant_lights[1]      color 0.4,0.4,0.5 / direction 0,0,1
 *   +0x40 point_light_count      0                            (hi16 of 0x00000000)
 *   +0x42 pad1                   0
 *   +0x44 point_light_indices    0, 0
 *   +0x4C reflection_tint_color  alpha 0.5 / rgb 1,1,1
 *   +0x5C shadow_vector          0, 0, -1                     (straight down)
 *   +0x68 shadow_color           0, 0, 0
 * NOTE the distant-light direction is the literal 0xBF13B646 = -0.577, NOT the 1/sqrt(3)
 * constant 0xBF13CD3A = -0.57735026 used elsewhere in this segment; it is not a unit vector. */

#include "../headers/render_lighting.h"

const render_lighting default_object_lighting =
{
    { { 0.2f, 0.2f, 0.2f } },                       /* ambient_color */
    2,                                              /* distant_light_count */
    0,                                              /* pad */
    {                                               /* distant_lights[2] */
        { { { 1.0f, 1.0f, 1.0f } }, { { -0.577f, -0.577f, -0.577f } } },
        { { { 0.4f, 0.4f, 0.5f } }, { {  0.0f,    0.0f,    1.0f    } } },
    },
    0,                                              /* point_light_count */
    0,                                              /* pad1 */
    { 0, 0 },                                       /* point_light_indices[2] */
    { { 0.5f, 1.0f, 1.0f, 1.0f } },                 /* reflection_tint_color (alpha, r, g, b) */
    { { 0.0f, 0.0f, -1.0f } },                      /* shadow_vector */
    { { 0.0f, 0.0f, 0.0f } },                       /* shadow_color */
};
