/* blip_colors @0x82126408 — .rdata, 84 bytes to the next symbol (aCDropship @0x8212645C);
 * DB applied type `const real_rgb_color[7]`. sizeof(real_rgb_color) == 12, so 84 == 7 entries ==
 * NUMBER_OF_BLIP_TYPES. render_blip passes &blip_colors[blip_type] straight to
 * rasterizer_hud_motion_sensor_blip_draw, so the index is a blip_type.
 * Big-endian words -> IEEE-754 (red, green, blue):
 *   +0x0000 3F800000 3F000000 00000000 -> {1.0, 0.5, 0.0}  _blip_type_self           orange
 *   +0x000C 3F800000 3F800000 00000000 -> {1.0, 1.0, 0.0}  _blip_type_friend         yellow
 *   +0x0018 3F800000 00000000 00000000 -> {1.0, 0.0, 0.0}  _blip_type_enemy          red
 *   +0x0024 3F800000 3F800000 00000000 -> {1.0, 1.0, 0.0}  _blip_type_vehicle_friend yellow
 *   +0x0030 3F800000 00000000 00000000 -> {1.0, 0.0, 0.0}  _blip_type_vehicle_enemy  red
 *   +0x003C 3F000000 3F000000 3F800000 -> {0.5, 0.5, 1.0}  _blip_type_custom         pale blue
 *   +0x0048 00000000 00000000 00000000 -> {0.0, 0.0, 0.0}  _blip_type_none           black
 *
 * DEVIATION: src/render_blip.c declares this `extern real_rgb_color blip_colors[]` — neither const
 * nor extent-bearing; the object is in .rdata and the DB applied type is `const real_rgb_color[7]`.
 * Defined const with the DB extent. */

#include "../headers/real_rgb_color.h"
#include "../headers/blip_type.h"

const real_rgb_color blip_colors[NUMBER_OF_BLIP_TYPES] =
{
    { { 1.0f, 0.5f, 0.0f } },   /* _blip_type_self */
    { { 1.0f, 1.0f, 0.0f } },   /* _blip_type_friend */
    { { 1.0f, 0.0f, 0.0f } },   /* _blip_type_enemy */
    { { 1.0f, 1.0f, 0.0f } },   /* _blip_type_vehicle_friend */
    { { 1.0f, 0.0f, 0.0f } },   /* _blip_type_vehicle_enemy */
    { { 0.5f, 0.5f, 1.0f } },   /* _blip_type_custom */
    { { 0.0f, 0.0f, 0.0f } }    /* _blip_type_none */
};
