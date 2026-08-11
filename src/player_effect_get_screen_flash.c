/* player_effect_get_screen_flash @0x83737A80 0x83737A80 — compute the screen-flash/fade overlay for a local player this
 * frame. A pending global screen fade (screen_fade.ticks != 0xFFFF, and either fading out or still within its
 * tick window) takes priority and produces a white-tinted fade whose intensity ramps via the cosine transition
 * function (_transition_function_cosine, inverted while fading in). Otherwise, once the fade has elapsed, the local player's own active screen flash
 * (from local_player_effect_data) is emitted, its intensity driven by the flash's fade function over its
 * remaining lifetime. No overlay is produced while the console is active.
 *
 * DEVIATION: the decompiler re-called game_time_get() once per fade-ratio comparison branch (it is idempotent
 * within a frame); computed once here. The fade ratio's >=0 / <=1 branch ladder reduces to clamp(ratio, 0, 1). */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/render_screen_flash.h"
#include "headers/render_screen_flash_type.h"
#include "headers/screen_flash_just_started_flags.h"
#include "headers/transition_function.h"
#include "headers/blam_data_globals.h"

extern uint8_t console_is_active(void);
extern int game_time_get(void);
extern int16_t game_time_get_elapsed(void);
extern float transition_function_evaluate(int16_t function_type, float value);

void player_effect_get_screen_flash(int16_t local_player_index, render_screen_flash *screen_flash)
{
    player_effect_globals_definition *globals;
    player_effect_datum *effect;
    float fade_ratio;

    if ( console_is_active() )
        return;

    globals = player_effect_globals;

    if ( (uint16_t)globals->screen_fade.ticks == 0xFFFF
      || (!globals->screen_fade.fading_out
          && game_time_get() - globals->screen_fade.start_time > globals->screen_fade.ticks) )
    {
        /* fade elapsed (or none) — emit the local player's own screen flash */
        if ( local_player_index == -1 )
            return;

        globals->screen_fade.ticks = -1;
        effect = &globals->local_player_effect_data[local_player_index];
        if ( effect->screen_flash_time_left > 0 || (effect->flags & (1u << _screen_flash_just_started_bit)) != 0 )
        {
            effect->flags &= ~(1u << _screen_flash_just_started_bit);
            screen_flash->type = render_screen_flash_type_map[effect->screen_flash.type];
            screen_flash->color.n[0] = effect->screen_flash.screen_flash_color.n[0];
            screen_flash->color.n[1] = effect->screen_flash.screen_flash_color.n[1];
            screen_flash->color.n[2] = effect->screen_flash.screen_flash_color.n[2];
            screen_flash->color.n[3] = effect->screen_flash.screen_flash_color.n[3];
            if ( effect->screen_flash.duration <= 0.0f )
                screen_flash->intensity = effect->screen_flash.zero_scale_factor;
            else
                screen_flash->intensity = transition_function_evaluate(
                    effect->screen_flash.fade_function,
                    (((float)effect->screen_flash_time_left / effect->screen_flash.duration)
                          * effect->screen_flash.zero_scale_factor));
            /* DEVIATION(restored dropped call): the binary winds the flash lifetime down each frame —
             * bl game_time_get_elapsed @0x83737D80 then sth @0x83737D8C into screen_flash_time_left;
             * this decrement was missing from the reconstruction. */
            effect->screen_flash_time_left -= game_time_get_elapsed();
        }
        return;
    }

    /* active screen fade */
    screen_flash->type = _render_screen_flash_type_lighten;
    screen_flash->color.n[1] = globals->screen_fade.color.n[0];
    screen_flash->color.n[2] = globals->screen_fade.color.n[1];
    screen_flash->color.n[0] = 1.0f;
    screen_flash->color.n[3] = globals->screen_fade.color.n[2];

    if ( globals->screen_fade.ticks <= 0 )
    {
        fade_ratio = 1.0f;
    }
    else
    {
        float ratio = (float)(game_time_get() - globals->screen_fade.start_time)
                    / (float)globals->screen_fade.ticks;
        if ( ratio < 0.0f )
            ratio = 0.0f;
        else if ( ratio > 1.0f )
            ratio = 1.0f;
        fade_ratio = transition_function_evaluate(_transition_function_cosine, ratio);
    }

    screen_flash->intensity = fade_ratio;
    if ( !globals->screen_fade.fading_out )
        screen_flash->intensity = (float)1.0 - fade_ratio;

    if ( screen_flash->intensity >= 0.0f )
    {
        if ( screen_flash->intensity > 1.0f )
            screen_flash->intensity = 1.0f;
    }
    else
    {
        screen_flash->intensity = 0.0f;
    }
}
