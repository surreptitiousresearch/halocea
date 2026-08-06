/* lighting_storage @ 0x8442A628 (.data, 116 bytes)
 * DB applied_types: render_lighting lighting_storage;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 ambient_color              = 0x000000000000000000000000
 *   +0x0C distant_light_count        = 0x0000
 *   +0x0E pad                        = 0x0000
 *   +0x10 distant_lights             = 0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x40 point_light_count          = 0x0000
 *   +0x42 pad1                       = 0x0000
 *   +0x44 point_light_indices        = 0x0000000000000000
 *   +0x4C reflection_tint_color      = 0x00000000000000000000000000000000
 *   +0x5C shadow_vector              = 0x000000000000000000000000
 *   +0x68 shadow_color               = 0x000000000000000000000000
 * data-bss zero-fill; DB applied-type `render_lighting lighting_storage;`.
 */
#include "../headers/render_lighting.h"

render_lighting lighting_storage;
