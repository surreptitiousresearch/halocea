/* hud_update_unit_local_player @0x836D6A80 — per-frame update of the shield-flash fade state for one local
 * player's unit HUD. Resolves the local player's controlled unit; on first sight it seeds the cached body and
 * shield vitalities. When the shield drops (took damage) it (re)starts the shield-hit timer if the fade is
 * idle, snaps the fade to 0 while the hit is fresh (< 15 ticks), otherwise advances the fade by elapsed/30.
 * When the shield is unchanged it advances an in-progress fade; when the shield rises (recharged) it clears the
 * fade to -1. It always restamps the shield-hit time afterward (except on the fresh-hit early out). Finally,
 * during cinematics it plays the unit's HUD sounds.
 *
 * The object record is reached through the salted object-header lookup; object+224 is body vitality and
 * object+228 is shield vitality. The int64/register puns in the decompiler are just int elapsed-tick ->
 * float / 30 conversions. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_hud_globals.h"
#include "headers/player_datum.h"
#include "headers/hud_scripted_globals.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int game_time_get(void);
extern uint8_t cinematic_in_progress(void);
extern void hud_play_unit_sounds(const player_datum *player, uint8_t show_hud);

void hud_update_unit_local_player(int16_t local_player_index)
{
    if ( local_player_get_player_index(local_player_index) != -1 )
    {
        int object_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                               local_player_get_player_index(local_player_index))->unit_index;
        if ( object_index != -1 )
        {
            unit_hud_state *state = &unit_hud_globals->hud_states[local_player_index];
            object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

            if ( state->last_body_vitality == -1.0f )
                state->last_body_vitality = object->object.body_vitality;
            if ( state->last_shield_vitality == -1.0f )
                state->last_shield_vitality = object->object.shield_vitality;

            float last_shield = state->last_shield_vitality;
            float current_shield = object->object.shield_vitality;

            if ( last_shield > current_shield )
            {
                if ( state->fade_time < 0.0f || state->fade_time > 1.0f )
                    state->last_shield_hit_time = game_time_get();
                if ( game_time_get() - state->last_shield_hit_time < 15 )
                {
                    state->fade_time = 0.0f;
                    goto done;
                }
                state->last_shield_vitality = object->object.shield_vitality;
                int elapsed = game_time_get() - state->last_shield_hit_time;
                state->fade_time = ((float)elapsed * SECONDS_PER_TICK) + state->fade_time;
            }
            else
            {
                state->last_shield_vitality = object->object.shield_vitality;
                if ( last_shield < current_shield )
                {
                    state->fade_time = -1.0f;
                }
                else if ( state->fade_time > 0.0f )
                {
                    int elapsed = game_time_get() - state->last_shield_hit_time;
                    state->fade_time = ((float)elapsed * SECONDS_PER_TICK) + state->fade_time;
                }
            }
            state->last_shield_hit_time = game_time_get();
        }
    }

done:
    if ( cinematic_in_progress() )
    {
        int player_index = local_player_get_player_index(local_player_index);
        if ( player_index != -1 )
            hud_play_unit_sounds(
                DATA_ARRAY_ELEMENT(player_data, player_datum, player_index),
                hud_scripted_globals->show_hud);
    }
}
