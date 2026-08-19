/* hud_draw_bitmap @0x837A0310 — forwards to hud_draw_bitmap_with_meter with no meter overlay.
 *
 * DEVIATION: the decompiler invented a bogus 30-parameter signature (a11..a29 unused ints, trailing
 * uninitialized a30); the DB prototype confirms the real signature is 10 parameters.
 *
 * DEVIATION: the forward drops one argument. hud_draw_bitmap_with_meter takes TEN parameters, and the
 * whole body of this thunk is `mr r11,r10` + `lbz r9, arg_57` + `mr r10,r9` + `stb r11, r1+0x57` +
 * `stb r9, r1+0x5F` (0x837A031C-0x837A0340): incoming color -> outgoing arg 8, incoming param 8
 * (in_multiplayer) -> outgoing arg 9, incoming param 9 (is_interface_bitmap) -> outgoing arg 10.
 * Incoming param 10 (is_crosshair_bitmap) is read by nothing here and forwarded nowhere; it is kept
 * in the signature because the call sites in crosshairs_draw.c pass it. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/real_rectangle2d.h"

typedef struct rasterizer_meter_parameters rasterizer_meter_parameters;

extern void hud_draw_bitmap_with_meter(rasterizer_meter_parameters *meter_parameters, const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color32, uint8_t in_multiplayer, uint8_t is_interface_bitmap);

void hud_draw_bitmap(const bitmap_data *bitmap, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const real_rectangle2d *clip, float scale, float theta, unsigned int color, uint8_t in_multiplayer, uint8_t is_interface_bitmap, uint8_t is_crosshair_bitmap)
{
    hud_draw_bitmap_with_meter(0, bitmap, absolute_placement, placement, clip, scale, theta, color,
            in_multiplayer, is_interface_bitmap);
}
