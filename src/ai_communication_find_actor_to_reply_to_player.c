/* ai_communication_find_actor_to_reply_to_player @0x837CF5B0 — for a player-spoken line, decide which AI actor
 * (if any) should reply and how strongly. Walks the global_reply_table run matching the line's vocalization and
 * damage category, rolls each row's player_chance, resolves a candidate replier (protagonist_type 2 = a found
 * ally, 3 = the target unit's actor, 4 = an "other" team-mate), then checks that actor's reply-event throttle:
 * computes a freshness rating (time since last spoken, scaled to [0,1]) and rejects the actor if its line is
 * still disabled. Returns the chosen actor index (or -1) and writes the rating to *reply_rating_reference.
 *
 * DEVIATION: the ai_communication_find_global_actor_to_talk calls have trailing arguments mis-attributed by
 * Hex-Rays — the float max_distance consumes r7 as its GPR shadow slot, so slots 5-7 are r8-r10 and slots 8-10
 * are the stack halfwords at +0x56/+0x5E/+0x66. IDA's per-register comments encode the SAME un-shifted map and
 * must not be transcribed; taken from the binary instead: li r8,-1 (ai_communication_type = "any"),
 * lhz r9,8(r31) = row->communication_priority, mr r10,r29 = speech_priority, then row->vocalization_type,
 * row->animation_type, 0 — matching the corrected control sites in ai_communication_event.c field for field. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/dialogue_usage_flags.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_globals.h"
#include "headers/reply_usage.h"
#include "headers/dialogue_event_status.h"
#include "headers/object_type.h"
#include "headers/ai_communication_protagonist_type.h"
#include "headers/blam_data_globals.h"

extern const reply_usage     global_reply_table[];
extern const int16_t         communication_speech_priorities[];

extern int ai_communication_find_global_actor_to_talk(int16_t team_index, int16_t find_actor_mode, int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t sound_scripted_dialog_is_playing(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int16_t actor_communication_team(int actor_index);
extern int game_time_get(void);

int ai_communication_find_actor_to_reply_to_player(int unit_index, int target_unit_index,
        int16_t vocalization_type, int16_t damage_category, float *reply_rating_reference)
{
    int chosen_actor = -1;
    float rating = 1.0f;

    if ( ai_globals->dialogue_triggers_enabled && vocalization_type != -1 )
    {
        const reply_usage *row = global_reply_table;
        int16_t reply_row_index = 0;
        while ( row->original_vocalization_type == vocalization_type )
        {
            if ( row->original_damage_category != -1 && row->original_damage_category != damage_category )
                goto next_row;

            int16_t speech_priority = communication_speech_priorities[row->communication_priority];
            if ( sound_scripted_dialog_is_playing() && (row->flags & (1u << _dialogue_usage_lookup_bit)) == 0 )
                goto next_row;
            if ( row->player_chance <= 0.0f )
                goto next_row;
            unsigned int *seed = get_global_random_seed_address();
            if ( real_seed_random(seed) >= row->player_chance )
                goto next_row;

            int actor = -1;
            unit_datum *unit = (unit_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
            switch ( (uint16_t)row->protagonist_type )
            {
                case _comm_protagonist_friend:
                    actor = ai_communication_find_global_actor_to_talk(
                        unit->object.owner_team_index, 1, unit_index, -1, 9.0f, -1,
                        row->communication_priority, speech_priority,
                        row->vocalization_type, row->animation_type, 0);
                    break;
                case _comm_protagonist_target:
                {
                    unit_datum *target_object = object_try_and_get_and_verify_type(target_unit_index, object_mask_unit);
                    if ( target_object )
                        actor = target_object->unit.actor_index;  /* target unit +0x1F4 controlling actor */
                    break;
                }
                case _comm_protagonist_enemy:
                    actor = ai_communication_find_global_actor_to_talk(
                        unit->object.owner_team_index, 2, unit_index, -1, 9.0f, -1,
                        row->communication_priority, speech_priority,
                        row->vocalization_type, row->animation_type, 0);
                    break;
                default:
                    break;
            }

            if ( actor != -1 )
            {
                int16_t team = actor_communication_team(actor);
                if ( team == -1 )
                    goto next_row;
                dialogue_event_status *event = &global_reply_events[2 * reply_row_index + team];
                int now = game_time_get();
                if ( event->last_time_spoken != -1 )
                {
                    rating = (float)(now - event->last_time_spoken) * 0.0011111111f;
                    if ( rating >= 0.0f )
                    {
                        if ( rating > 1.0f )
                            rating = 1.0f;
                    }
                    else
                    {
                        rating = 0.0f;
                    }
                }
                if ( event->disable_until_time != -1 && event->disable_until_time - now > 0 )
                    actor = -1;  /* throttled */
            }

            chosen_actor = actor;
            if ( chosen_actor != -1 )
                break;

next_row:
            ++row;
            ++reply_row_index;
        }
    }

    if ( reply_rating_reference )
        *reply_rating_reference = rating;
    return chosen_actor;
}
