/* internal_rasterize_target_name 0x8374C010 — render the name of the closest player to the local
 * player with a fade-in / fade-out alpha. Uses hysteresis: multiplayer.player_display_index stores the
 * displayed player index, multiplayer.player_display_count stores a 0-15 fade counter that ramps up when
 * the closest player matches and
 * down otherwise; the displayed index is only updated when the counter reaches 0. Alpha is capped
 * at 10/15 of the ramp, raised to the power 1.9, then halved.
 * DEVIATION: decompiler wrote pow(v10, v9) — disasm shows pow(f1=fade, f2=1.9); args corrected. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include <math.h>
#include "headers/blam_data_globals.h"


extern int find_closest_player_index(int player_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void game_engine_rasterize_message(const wchar_t *string, float alpha);

void internal_rasterize_target_name(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    int closest_player_index = -1;
    if ( player->local_player_index != (uint16_t)0xFFFF && player->unit_index != -1 )
    {
        closest_player_index = find_closest_player_index(player_index);
        if ( closest_player_index == player_index )
            closest_player_index = -1;
    }

    int *displayed_index = &player->multiplayer.player_display_index; /* reused: displayed player index */
    int *fade_timer      = &player->multiplayer.player_display_count; /* reused: fade counter 0..15 */

    int timer = *fade_timer;
    if ( *displayed_index == closest_player_index )
    {
        if ( timer < 15 )
            *fade_timer = timer + 1;
    }
    else
    {
        if ( timer > 0 )
            *fade_timer = timer - 1;
        if ( !*fade_timer )
            *displayed_index = closest_player_index;
    }

    int target_player_index = *displayed_index;
    if ( target_player_index == -1 )
        return;

    player_datum *target = datum_try_and_get(player_data, target_player_index);
    if ( !target )
    {
        *displayed_index = -1;
        return;
    }

    wchar_t name_buffer[12];
    memset(name_buffer, 0, sizeof(name_buffer));

    int timer_capped = *fade_timer;
    if ( timer_capped >= 10 )
        timer_capped = 10;

    float fade  = (float)timer_capped * 0.1f;
    float alpha = (float)pow((double)fade, 1.9) * 0.5f;

    ustrncpy(name_buffer, target->name, 11);
    name_buffer[11] = 0;
    game_engine_rasterize_message(name_buffer, alpha);
}
