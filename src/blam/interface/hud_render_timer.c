/* hud_render_timer @0x836A43A8 — render the scripted HUD countdown timer as three number elements
 * (MM : SS . HH) anchored to a screen corner. Active only while hud_messaging_globals->timer.enabled.
 * The digit advance between fields is derived from a font-character width in the timer's interface tag.
 * Flashing (and the reference-time latch used for the flash animation) kicks in once the remaining time
 * drops to or below flash_cutoff (or immediately, when expired); expired also resets ticks to -1 and
 * swaps in the "expired" color block.
 *
 * DEVIATIONS (verified against disassembly — the decompiler mangled the int<->float and 128-bit shuffles):
 *   - `v3` digit advance: a signed font-width byte converted to float, *2.0, back to int. The decompiler
 *     rendered the fcfid/fctiwz round-trip with a phantom HIDWORD(...)=0x82000000; it is a plain
 *     (float)(signed char) cast.
 *   - The two 8-int copy loops are 32-byte hud_color_definition copies into numbers.colors, sourced from
 *     fixed offsets inside the (partially-modeled) hud_globals tag: +0x360 (running) / +0x380 (expired).
 *   - Between the three hud_draw_numbers calls the X offset is advanced by `(double)v3 * 2.5`, computed
 *     once and reused; the decompiler expressed each advance as an __int128 word-shuffle with magic
 *     constants 0x88880000 / 0x4004000000000000(==2.5). Restored as a double add + int truncation.
 *   - `((v6==0)+((unsigned)v6>>31)-1)&v6` is max(0, v6) (clamp the tick count to non-negative). The
 *     seconds/minutes/hundredths divisions appear as reciprocal-multiply (0x88888889) idioms; restored as
 *     plain integer divides. */

#include <stdint.h>
#include <string.h>
#include "headers/point2d.h"
#include "headers/render_globals.h"
#include "headers/hud_messaging_globals.h"
#include "headers/hud_globals.h"
#include "headers/hud_color_definition.h"
#include "headers/hud_absolute_placement_definition.h"
#include "headers/number_hud_element_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/hud_corner.h"
#include "headers/interface_tag_index.h"
#include "headers/hud_number_definition.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern int16_t scripted_hud_get_timer_ticks(void);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern void hud_draw_numbers(int16_t local_player_index, const hud_absolute_placement_definition *placement, const number_hud_element_definition *numbers, int16_t value, int16_t decimal_value, int16_t draw_flags, int flash_reference_time, float override_scale);

void hud_render_timer(void)
{
    hud_timer_data_definition *timer = &hud_messaging_globals->timer;
    if (!timer->enabled)
        return;

    hud_absolute_placement_definition placement;
    placement.corner = timer->corner;
    memset(&placement.pad, 0, 0x22);  /* zero the remaining 34 bytes (pad + unused[8]) */

    int current_time = game_time_get();
    int16_t flash = 0;
    int16_t digit_advance = 0;
    int16_t timer_ticks = scripted_hud_get_timer_ticks();

    number_hud_element_definition numbers;
    numbers.digits = 2;
    numbers.fractional_digits = 4;
    numbers.number_flags = 1;
    numbers.placement.offset = timer->position;
    numbers.placement.scale.n[0] = 1.0f;
    numbers.placement.scale.n[1] = 1.0f;

    /* digit advance = 2 * (signed font-width byte at +0x11 of the timer's interface tag) */
    int tag_index = interface_get_tag_index(_interface_hud_digits);
    if (tag_index != -1)
    {
        const hud_number_definition *interface_definition =
            TAG_GET(hud_number_definition, tag_index);
        digit_advance = (int16_t)(int)((float)(signed char)interface_definition->screen_width * 2.0f);
    }

    /* corner-relative horizontal positioning of the field group */
    switch ((uint16_t)timer->corner)
    {
        case _hud_corner_top_left:
        case _hud_corner_bottom_left:
            break;
        case _hud_corner_top_right:
        case _hud_corner_bottom_right:
            numbers.placement.offset.n[0] += 5 * digit_advance;
            digit_advance = -digit_advance;
            break;
        default:
            numbers.placement.offset.n[0] -= 3 * digit_advance;
            break;
    }

    if (timer_ticks > 0)
    {
        numbers.colors = hud_globals->timer_definition.color;  /* recovered: *(hud_color_definition *)((char *)hud_globals + 0x360) -> timer_definition.color (running) */

        int16_t flash_cutoff = timer->flash_cutoff;
        if (timer_ticks <= flash_cutoff)
        {
            flash = 1;
            if (timer->ticks > flash_cutoff)
            {
                timer->ticks = flash_cutoff;
                timer->reference_time = flash_cutoff - timer_ticks + current_time;
            }
        }
    }
    else
    {
        timer->ticks = -1;
        numbers.colors = hud_globals->timer_definition.time_up_color;  /* recovered: *(hud_color_definition *)((char *)hud_globals + 0x380) -> timer_definition.time_up_color (expired) */
        flash = 1;
        if (timer->reference_time == -1)
            timer->reference_time = game_time_get();
    }

    int16_t clamped_ticks = (int16_t)(timer_ticks < 0 ? 0 : timer_ticks);  /* max(0, timer_ticks) */
    int16_t total_seconds = clamped_ticks / 30;
    double advance = (double)digit_advance * 2.5;

    /* minutes */
    hud_draw_numbers(render.local_player_index, &placement, &numbers, total_seconds / 60, -1, flash,
                     timer->reference_time, 2.0f);

    /* seconds */
    numbers.placement.offset.n[0] =
        (int16_t)(int)((double)numbers.placement.offset.n[0] + advance);
    hud_draw_numbers(render.local_player_index, &placement, &numbers, total_seconds % 60, -1, flash,
                     timer->reference_time, 2.0f);

    /* hundredths */
    numbers.placement.offset.n[0] =
        (int16_t)(int)((double)numbers.placement.offset.n[0] + advance);
    hud_draw_numbers(render.local_player_index, &placement, &numbers, 100 * (clamped_ticks % 30) / 30, -1, flash,
                     timer->reference_time, 2.0f);
}
