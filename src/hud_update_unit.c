/* hud_update_unit @0x836D8258 — advance the shield/body HUD fade state for every local player's unit each
 * frame. For each local player with a live unit, the unit_hud_state (one per local player) tracks the last
 * seen shield/body vitality and a fade timer driven by recent shield damage:
 *   - shield dropped this frame: (re)start the hit timer if the fade is out of [0,1]; once at least 15
 *     ticks have elapsed since the last hit, latch the new shield value and advance the fade by
 *     elapsed/30, restamping the hit time; otherwise reset the fade to 0.
 *   - shield unchanged: keep advancing the fade by elapsed/30 while it is positive.
 *   - shield recovering: clear the fade to -1.
 * Regardless, while a cinematic is in progress the unit HUD sounds are played for the player (gated by the
 * scripted show_hud flag).
 *
 * DEVIATION: the decompiler could not structure the goto/break web (the shield-steady/recovering case is
 * reached by `break` and rejoins the loop via a shared LABEL_19/LABEL_20); it is reconstructed here as a
 * single if/else on the shield comparison. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_hud_globals.h"
#include "headers/hud_scripted_globals.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern int game_time_get(void);
extern uint8_t cinematic_in_progress(void);
extern void hud_play_unit_sounds(const player_datum *player, uint8_t show_hud);

void hud_update_unit(void)
{
    __int16 local_player_index = local_player_get_next(-1);
    if (local_player_index == -1)
        return;

    do
    {
        int player_index = local_player_get_player_index(local_player_index);
        if (player_index != -1)
        {
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
            /* recovered: *(int *)(player_data->data + datum(player_index) + 52) -> player->unit_index */
            int unit_datum_index = player->unit_index;
            if (unit_datum_index != -1)
            {
                unit_hud_state *hud = &unit_hud_globals->hud_states[local_player_index];
                unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_datum_index)->datum);

                if (hud->last_body_vitality == -1.0f)
                    hud->last_body_vitality = unit_object->object.body_vitality;
                if (hud->last_shield_vitality == -1.0f)
                    hud->last_shield_vitality = unit_object->object.shield_vitality;

                float last_shield = hud->last_shield_vitality;
                float current_shield = unit_object->object.shield_vitality;

                if (last_shield > current_shield) /* shield dropped this frame */
                {
                    if (hud->fade_time < 0.0f || hud->fade_time > 1.0f)
                        hud->last_shield_hit_time = game_time_get();
                    if (game_time_get() - hud->last_shield_hit_time >= 15)
                    {
                        hud->last_shield_vitality = current_shield;
                        hud->fade_time += (float)(game_time_get() - hud->last_shield_hit_time) * SECONDS_PER_TICK;
                        hud->last_shield_hit_time = game_time_get();
                    }
                    else
                    {
                        hud->fade_time = 0.0f;
                    }
                }
                else /* shield steady or recovering */
                {
                    hud->last_shield_vitality = current_shield;
                    if (last_shield >= current_shield) /* unchanged */
                    {
                        if (hud->fade_time > 0.0f)
                            hud->fade_time += (float)(game_time_get() - hud->last_shield_hit_time) * SECONDS_PER_TICK;
                    }
                    else /* recovering */
                    {
                        hud->fade_time = -1.0f;
                    }
                    hud->last_shield_hit_time = game_time_get();
                }
            }
        }

        if (cinematic_in_progress())
        {
            int sound_player_index = local_player_get_player_index(local_player_index);
            if (sound_player_index != -1)
                hud_play_unit_sounds(
                    DATA_ARRAY_ELEMENT(player_data, player_datum, sound_player_index),
                    hud_scripted_globals->show_hud);
        }

        local_player_index = local_player_get_next(local_player_index);
    } while (local_player_index != -1);
}
