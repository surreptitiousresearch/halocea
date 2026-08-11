/* ai_communication_event @0x837D0040 — the central AI dialogue dispatcher. Given a communication event
 * (a type plus a subject unit, a cause unit, hostility, damage and information context), it:
 *   1. Resolves the subject/cause objects -> their teams, races, actors, encounters and "group" membership
 *      booleans, and detects ally-fire / allegiance incidents (escalating hostility to "enemy").
 *   2. Builds the enemy-status condition booleans for the subject actor (alive, can-see-enemy, recently-saw,
 *      etc.) used to gate which lines may play.
 *   3. Computes per-team/per-dialogue speech-throttle availability from the recent shout/talk/chatter timers
 *      and communication_timer_tolerances.
 *   4. Walks the contiguous run of global_dialogue_table rows for this communication_type, filtering each by
 *      hostility / enemy-status / race / damage / group / scripted-dialog rules, resolving the protagonist
 *      (and reply) actor, applying player-rating and recent-spoken decay, and accumulating qualifying lines
 *      with a selection weight into a candidate buffer (up to 16).
 *   5. Weight-randomly selects one candidate (or, for interrupts, the flagged one) and either notifies/finishes
 *      it (reply path) or speaks it via unit_speak, orienting the speaker's head toward the target and issuing
 *      the look/animation impulses and speech-timer updates.
 *
 * Opaque object/actor/encounter records are accessed at raw offsets per the established convention. The
 * candidate buffer, per-team availability scratch and timer scratch are kept as raw byte buffers because the
 * decompiler aliases them (the original reuses the unit_speech_item storage as the availability array before
 * the line is selected); they do not overlap in time, so distinct buffers are behaviourally equivalent.
 *
 * Deviations: Hex-Rays widens single-precision math to double (fpN) and packs pairs of values into __int64
 * temporaries (v73/v77) — reconstructed as plain float/scalar locals. */

#include <stdint.h>
#include "headers/ppc_intrinsics.h"
#include "headers/ai_communication_candidate.h"
#include "headers/dialogue_usage_flags.h"
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/ai_globals.h"
#include "headers/dialogue_usage.h"
#include "headers/dialogue_event_status.h"
#include "headers/unit_speech_item.h"   /* also pulls ai_information_packet + its ai_information_data */
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/actor_action.h"
#include "headers/actor_target_type.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/ai_communication_look_direction.h"
#include "headers/ai_communication_priority.h"
#include "headers/actor_race.h"
#include "headers/actor_mode.h"
#include "headers/actor_combat_status.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/ai_communication_protagonist_type.h"
#include "headers/unit_speech_priority.h"
#include "headers/ai_information_look_type.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);
extern int _cntlzw(unsigned int);

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);


extern const dialogue_usage   global_dialogue_table[];
extern const int16_t          communication_protagonist_default_look_priorities[];
extern const int16_t          communication_recipient_default_look_priorities[];
extern const float            communication_notification_delays[];
extern const float            communication_play_delays[];
extern const int16_t          communication_player_speaking_priorities[];
extern const int16_t          communication_speech_priorities[];
extern const float            communication_timer_tolerances[8][2][5];  /* [priority/dialogue][group][5] */

extern int game_time_get(void);
extern int16_t ai_get_race_from_team_index(int16_t team_index);
extern uint16_t actor_type_get_race(int16_t actor_type);
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int ai_communication_find_global_actor_to_talk(int16_t team_index, int16_t find_actor_mode, int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);
extern int ai_communication_find_specific_actor_to_talk(int ai_index, int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);
extern int ai_communication_find_actor_to_reply_to_player(int unit_index, int target_unit_index, int16_t vocalization_type, int16_t damage_category, float *reply_rating_reference);
extern uint8_t game_allegiance_incident(int16_t aggressor_team_index, int16_t victim_team_index, int16_t type, uint8_t *notify_immediately);
extern void ai_handle_allegiance_broken_notification(int16_t team1_index, int16_t team2_index, uint8_t broken);
extern uint8_t sound_scripted_dialog_is_playing(void);
extern float ai_communication_get_player_rating(int unit_index, uint8_t test_line_of_sight, int *unit_index_reference, float *distance_reference);
extern int16_t actor_communication_team(uint16_t actor_index);
extern int16_t ai_communication_consider_speech(int unit_index, int16_t communication_priority, int16_t speech_priority, int16_t delay_ticks, uint8_t allow_vocalization_lookup, uint8_t allow_recent_disabling, int16_t *vocalization_type, float *weight, int *sound_definition_index_reference, char *debugstring);
extern uint8_t unit_test_animation_impulse(int unit_index, int16_t animation_impulse);
extern int16_t actor_action_class(uint16_t actor_index);
extern void ai_communication_notify(int unit_index, int16_t priority, int16_t vocalization_type, ai_information_packet *ai_information);
extern void ai_communication_finished(int unit_index, int16_t priority, int16_t vocalization_type, uint8_t reply_to_player, int preselected_reply_actor_index, ai_information_packet *ai_information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern int unit_start_animation_impulse(int unit_index, int16_t animation_impulse, real_vector2d *alignment_vector);
extern void ai_communication_look_secondary_at_unit(int actor_index, int16_t type, int16_t priority, int look_unit_index, int prop_index);
extern void ai_communication_update_speech_timers(int unit_index, int16_t priority, int16_t vocalization_type, int16_t dialogue_type_index);

/* helper: object data pointer from a unit/object index (3-dword header records) */
static object_datum *object_from_index(int index)
{
    return DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, index)->datum;
}

void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index,
                            int16_t hostility, int16_t damage_type, int16_t information_type,
                            ai_information_data *information_data)
{
    int event_time = game_time_get();
    int now = event_time;
    int16_t candidate_count = 0;
    char any_interrupt = 0;
    int16_t damage_index = damage_type == -1 ? 0 : damage_type;
    int16_t hostility_level = hostility == -1 ? 0 : hostility;

    actor_datum *subject_actor = nullptr;
    actor_datum *cause_actor = nullptr;
    encounter_datum *subject_encounter = nullptr;
    unit_datum *subject_object = 0;
    unit_datum *cause_object = 0;
    int16_t subject_team = -1;
    int16_t cause_team = -1;
    int subject_actor_index = -1;
    int cause_actor_index = -1;
    int subject_encounter_index = -1;
    int16_t subject_race = 0;
    int16_t cause_race = 0;
    float total_weight = 0.0f;
    int global_actor_to_talk = -1;
    char need_global_actor = 1;       /* v176 */
    char need_other_actor = 1;        /* v181 */

    /* group-membership booleans: [0] in-actor-group, [1] commander/leader */
    char subject_groups[2] = { 0, 0 };  /* v177/v178 */
    char cause_groups[2]   = { 0, 0 };  /* v179/v180 */

    /* ---- resolve subject ---- */
    if ( subject_unit_index != -1 )
    {
        subject_object = (unit_datum *)object_from_index(subject_unit_index);
        subject_team = subject_object->object.owner_team_index;
        subject_actor_index = subject_object->unit.actor_index;
        subject_race = ai_get_race_from_team_index(subject_team);
        if ( subject_actor_index != -1 )
        {
            subject_actor = DATUM_GET(actor_data, actor_datum, subject_actor_index);
            subject_encounter_index = subject_actor->meta.encounter_index;
            subject_race = actor_type_get_race(subject_actor->meta.type);
            if ( subject_actor->situation.close_friends > 0 )
                subject_groups[1] = 1;
            else if ( subject_actor->situation.area_friends > 0 )
                subject_groups[1] = 0;
            if ( subject_actor->situation.close_friends > 0 || subject_actor->situation.area_friends > 0 )
                subject_groups[0] = 1;
            if ( subject_encounter_index != -1 )
                subject_encounter = DATUM_GET(encounter_data, encounter_datum, subject_encounter_index);
        }
        else if ( subject_object->unit.player_index != -1 )
        {
            subject_race = _race_player;
        }
    }

    /* ---- resolve cause ---- */
    if ( cause_unit_index != -1 )
    {
        cause_object = (unit_datum *)object_from_index(cause_unit_index);
        cause_team = cause_object->object.owner_team_index;
        cause_actor_index = cause_object->unit.actor_index;
        cause_race = ai_get_race_from_team_index(cause_team);
        if ( cause_actor_index == -1 )
        {
            if ( cause_object->unit.player_index != -1 )
                cause_race = _race_player;
        }
        else
        {
            cause_actor = DATUM_GET(actor_data, actor_datum, cause_actor_index);
            cause_race = actor_type_get_race(cause_actor->meta.type);
            if ( cause_actor->situation.close_friends > 0 )
            {
                cause_groups[0] = 1;
                cause_groups[1] = 1;
            }
            else if ( cause_actor->situation.area_friends > 0 )
            {
                cause_groups[0] = 1;
                cause_groups[1] = 0;
            }
        }
    }

    /* ---- ally-fire / allegiance handling ---- */
    if ( subject_object && cause_object && subject_team != cause_team && game_team_is_ally(subject_team, cause_team) )
    {
        char escalate_allegiance = 0;
        char want_incident = 0;
        if ( communication_type == _ai_communication_death )
        {
            if ( hostility_level == _comm_hostility_enemy )
            {
                escalate_allegiance = 1;
                want_incident = 1;
            }
            else
            {
                char subject_recently_active = 0;
                if ( subject_encounter )
                {
                    int enemy_visible_timer;
                    if ( subject_encounter->enemy_traitor || (enemy_visible_timer = subject_encounter->enemy_visible_timer) == -1 || enemy_visible_timer >= 270 )
                        subject_recently_active = 1;
                }
                global_actor_to_talk = ai_communication_find_global_actor_to_talk(
                    subject_team, 0, subject_unit_index, cause_unit_index, 18.0f,
                    communication_type /* _ai_communication_death in this branch */, 0, _unit_speech_scripted, -1, 0, 0);
                if ( global_actor_to_talk != -1 )
                {
                    need_global_actor = 0;
                    want_incident = 1;
                }
                if ( damage_index >= 3 && (damage_index <= 4 || damage_index == 9) )
                    want_incident = subject_recently_active == 0 ? 0 : want_incident;
                escalate_allegiance = damage_index == 3 ? 0 : subject_recently_active;
            }
            if ( escalate_allegiance )
                hostility_level = _comm_hostility_traitor;
            if ( want_incident )
            {
                uint8_t notify_state = 0;
                uint8_t broken = game_allegiance_incident(cause_team, subject_team,
                                            escalate_allegiance != 0, &notify_state);
                if ( notify_state )
                    ai_handle_allegiance_broken_notification(cause_team, subject_team, broken);
            }
        }
        if ( game_team_is_enemy(subject_team, cause_team) )
            hostility_level = _comm_hostility_traitor;
    }

    /* ---- enemy-status condition booleans for the subject actor ---- */
    unsigned char enemy_status_flags[6];
    if ( subject_actor )
    {
        char alive = (_cntlzw(subject_actor->target.any_target_ever) & 0x20) != 0;
        if ( subject_encounter )
        {
            int encounter_age = subject_encounter->enemy_visible_timer;
            enemy_status_flags[0] = alive;
            enemy_status_flags[1] = !(encounter_age == -1 || subject_encounter->enemy_alive);
            enemy_status_flags[2] = !((encounter_age != -1 && encounter_age < 180) || !subject_encounter->enemy_alive);
            int16_t enemies_seen = subject_actor->state.combat_status;
            enemy_status_flags[3] = enemies_seen < _actor_combat_status_definite && (encounter_age == -1 || encounter_age >= 75)
                                    && (subject_encounter->enemy_alive || enemies_seen > _actor_combat_status_none);
            enemy_status_flags[4] = enemies_seen < _actor_combat_status_dangerous && (encounter_age == -1 || encounter_age >= 75);
            if ( !subject_encounter->enemy_visible )  /* off 0x45 = enemy_visible (encounter_datum) */
                enemy_status_flags[5] = 0;
            else
                enemy_status_flags[5] = subject_encounter->enemy_alive == 0;
        }
        else
        {
            int can_see_enemy = subject_actor->target.target_really_alive;
            enemy_status_flags[0] = alive;
            enemy_status_flags[1] = !(can_see_enemy || subject_actor->target.since_any_target_visible_timer == -1);
            int last_seen = subject_actor->target.target_prop_index;
            int last_seen_ticks = subject_actor->target.since_any_target_visible_timer;
            enemy_status_flags[2] = (last_seen == -1 || last_seen_ticks == -1 || last_seen_ticks >= 180);
            int16_t enemies_seen = subject_actor->state.combat_status;
            enemy_status_flags[3] = enemies_seen < _actor_combat_status_definite
                                    && (subject_actor->target.since_any_target_visible_timer == -1 || subject_actor->target.since_any_target_visible_timer >= 75)
                                    && (can_see_enemy || enemies_seen > _actor_combat_status_none);
            enemy_status_flags[4] = ((unsigned int)enemies_seen < _actor_combat_status_dangerous);
            if ( subject_actor->target.target_type < actor_target_visible_enemy )
                enemy_status_flags[5] = 0;
            else
                enemy_status_flags[5] = can_see_enemy == 0;
        }
    }
    else
    {
        for ( int i = 0; i < 6; ++i )
            enemy_status_flags[i] = 1;
    }

    /* ---- hostility bitmap ---- */
    unsigned char hostility_flags[8] = { 0 };
    if ( hostility_level != -1 )
    {
        hostility_flags[hostility_level] = 1;
        if ( hostility_level == _comm_hostility_traitor )
            hostility_flags[0] = 1;
    }

    /* ---- speech-throttle availability per team/dialogue/group ---- */
    unsigned char speech_available[32] = { 0 };  /* [2 teams][6 dialogue][2 groups] flattened */
    /* DEVIATION: was a fused `unsigned char timer_scratch[72]` slot with int16 entries at byte +8
     * (first 8 bytes never referenced); untangled to a typed per-[team][dialogue][group] int16 array */
    int16_t timer_scratch[32] = { 0 };
    ai_globals_t *globals = ai_globals;
    for ( int team = 0; team < 2; team = (int16_t)(team + 1) )
    {
        int since_shout   = now - globals->last_shout_time[team];
        int since_talk    = now - globals->last_talk_time[team];
        int since_chatter = now - globals->last_chatter_time[team];
        /* clamp negatives to 0 (decompiler spelled the sign test as a __CFADD__ carry probe) */
        int16_t talk_age    = since_talk < 0 ? 0 : since_talk;
        int16_t shout_age   = since_shout < 0 ? 0 : since_shout;
        int16_t chatter_age = since_chatter < 0 ? 0 : since_chatter;

        for ( int dialogue = 0; dialogue <= 5; dialogue = (int16_t)(dialogue + 1) )
        {
            for ( int group = 0; group < 2; group = (int16_t)(group + 1) )
            {
                char available = 0;
                int16_t scratch_value = 0;

                float shout_tol = communication_timer_tolerances[dialogue][group][0];
                if ( shout_tol > 0.0f )
                {
                    int remaining = (int)(shout_tol * 30.0f - (float)chatter_age);
                    if ( (int16_t)remaining > 0 )
                    {
                        available = 1;
                        scratch_value = remaining < 0 ? 0 : (int16_t)remaining;
                    }
                }
                float talk_tol = communication_timer_tolerances[dialogue][group][1];
                if ( talk_tol > 0.0f )
                {
                    int remaining = (int)(talk_tol * 30.0f - (float)talk_age);
                    if ( (int16_t)remaining > 0 )
                    {
                        available = 1;
                        if ( scratch_value <= (int16_t)remaining )
                            scratch_value = (int16_t)remaining;
                    }
                }
                float chatter_tol = communication_timer_tolerances[dialogue][group][3];
                if ( chatter_tol > 0.0f )
                {
                    int remaining = (int)(chatter_tol * 30.0f - (float)shout_age);
                    if ( (int16_t)remaining > 0 )
                    {
                        available = 1;
                        if ( scratch_value <= (int16_t)remaining )
                            scratch_value = (int16_t)remaining;
                    }
                }
                if ( available )
                {
                    float floor_tol = communication_timer_tolerances[dialogue][group][4];
                    if ( floor_tol > 0.0f )
                    {
                        int slot = 2 * (8 * team + dialogue) + group;
                        int16_t stored = timer_scratch[slot];
                        if ( (float)stored < (floor_tol * 30.0f) )
                            available = 0;
                    }
                }
                int idx = 2 * (8 * team + dialogue) + group;
                speech_available[idx] = available;
                timer_scratch[idx] = scratch_value;
            }
        }
    }

    if ( !globals->dialogue_triggers_enabled )
        return;

    int16_t table_row_index = global_communication_table_indices[communication_type];
    if ( table_row_index == -1 )
        return;

    const dialogue_usage *row = &global_dialogue_table[table_row_index];
    if ( row->communication_type != communication_type )
        return;

    /* candidate accumulation buffer: up to 16 records of 56 bytes */
    ai_communication_candidate candidates[16];  /* 56-byte records; prior pass used a raw 984-byte blob */

    do
    {
        int16_t priority = row->communication_priority;

        if ( row->required_hostility != -1 && !hostility_flags[row->required_hostility] )
            goto next_row;
        if ( sound_scripted_dialog_is_playing() && row->communication_priority < _ai_communication_priority_yell && (row->flags & (1u << _dialogue_usage_override_scripted_bit)) == 0 )
            goto next_row;
        if ( row->required_enemy_status != -1 && !enemy_status_flags[row->required_enemy_status] )
            goto next_row;
        if ( row->required_subject_race != -1
          && (subject_unit_index == -1
           || ((uint16_t)subject_race & (uint16_t)row->required_subject_race) == 0) )
            goto next_row;
        if ( row->required_cause_race != -1
          && (cause_unit_index == -1
           || ((uint16_t)cause_race & (uint16_t)row->required_cause_race) == 0) )
            goto next_row;
        if ( row->required_damage != -1 && row->required_damage != damage_index )
            goto next_row;

        unsigned int protagonist_type = (uint16_t)row->protagonist_type;
        int16_t candidate_scratch = 0;
        float repeat_factor = 1.0f;       /* v98 */
        float reply_rating = 1.0f;        /* v211 */
        actor_datum *protagonist_actor = nullptr;
        int16_t look_target_type = 0;     /* v100 */
        int16_t recipient_look_priority = 0;
        char *protagonist_group_flags = nullptr;
        int16_t speech_priority = communication_speech_priorities[priority];
        char is_reply = 0;                /* v104 */
        int protagonist_unit_index = -1;  /* v106 */
        int protagonist_actor_index = -1; /* v105 */
        int recipient_unit_index = -1;    /* v107 */
        int look_unit_index = -1;         /* v88 */
        int reply_actor_index = -1;       /* v108 */
        float player_rating = 1.0f;       /* v114 */

        if ( protagonist_type > 4 )
            goto resolve_done;

        if ( protagonist_type == _comm_protagonist_cause )
        {
            protagonist_unit_index = cause_unit_index;
            protagonist_group_flags = cause_groups;
            protagonist_actor_index = cause_actor_index;
            protagonist_actor = cause_actor;
            recipient_unit_index = subject_unit_index;
            goto resolve_done;
        }
        if ( protagonist_type == _comm_protagonist_friend )
        {
            int actor_to_talk;
            recipient_unit_index = cause_unit_index;
            if ( need_global_actor )
            {
                if ( subject_encounter_index == -1 )
                    actor_to_talk = ai_communication_find_global_actor_to_talk(
                        subject_team, 1, subject_unit_index, cause_unit_index, 10.0f, row->animation_type,
                        communication_type, priority, speech_priority, 0, 0);
                else
                    actor_to_talk = ai_communication_find_specific_actor_to_talk(
                        (uint16_t)subject_encounter_index, subject_unit_index, cause_unit_index, 10.0f,
                        cause_unit_index, communication_type, priority,
                        communication_speech_priorities[priority],
                        row->vocalization_type, 0);
                global_actor_to_talk = actor_to_talk;
                need_global_actor = 0;
            }
            else
            {
                actor_to_talk = global_actor_to_talk;
            }
            protagonist_actor_index = actor_to_talk;
            if ( actor_to_talk != -1 )
            {
                protagonist_actor = DATUM_GET(actor_data, actor_datum, actor_to_talk);
                protagonist_unit_index = protagonist_actor->meta.unit_index;
            }
            goto resolve_done;
        }
        if ( protagonist_type == _comm_protagonist_target || protagonist_type == _comm_protagonist_subject )
        {
            protagonist_unit_index = subject_unit_index;
            protagonist_group_flags = subject_groups;
            protagonist_actor_index = subject_actor_index;
            protagonist_actor = subject_actor;
            recipient_unit_index = cause_unit_index;
            goto resolve_done;
        }
        /* protagonist_type == _comm_protagonist_enemy: find the "other" actor on the subject team */
        {
            int actor_to_talk;
            recipient_unit_index = cause_unit_index;
            if ( need_other_actor )
            {
                actor_to_talk = ai_communication_find_global_actor_to_talk(
                    subject_team, 2, subject_unit_index, cause_unit_index, 12.0f, row->animation_type,
                    communication_type, priority, speech_priority, 0, 0);
                need_other_actor = 0;
                subject_actor_index = actor_to_talk;  /* v206 reuse */
            }
            else
            {
                actor_to_talk = subject_actor_index;  /* v206 */
            }
            protagonist_actor_index = actor_to_talk;
            if ( actor_to_talk != -1 )
            {
                protagonist_actor = DATUM_GET(actor_data, actor_datum, actor_to_talk);
                protagonist_unit_index = protagonist_actor->meta.unit_index;
            }
        }

resolve_done:
        {
            char reject = 0;
            if ( protagonist_unit_index != -1 )
            {
                unit_datum *protagonist_object = (unit_datum *)object_from_index(protagonist_unit_index);
                if ( (protagonist_object->object.damage_flags & (1u << _object_dead_bit)) != 0 || protagonist_object->object.type == object_type_vehicle )
                    reject = 1;
                else if ( protagonist_object->unit.player_index == -1 || protagonist_object->unit.actor_index != -1 )
                {
                    /* fall through to LABEL_167 (no reject from this branch) */
                }
                else if ( (row->flags & (1u << _dialogue_usage_player_bit)) == 0 )
                    reject = 1;
                else
                    is_reply = 1;
            }
            else
            {
                reject = 1;
            }

            if ( protagonist_actor
              && (!protagonist_actor->state.mode
               || (protagonist_actor->state.action == actor_action_obey && !protagonist_actor->state.action_data.___u0.obey.allow_communication)) )
                reject = 1;
            if ( reject )
                goto next_row;
        }

        if ( is_reply )
        {
            reply_actor_index = ai_communication_find_actor_to_reply_to_player(
                protagonist_unit_index, recipient_unit_index, row->vocalization_type, damage_index, &reply_rating);
            if ( protagonist_unit_index == subject_unit_index )
            {
                global_actor_to_talk = reply_actor_index;
                need_global_actor = 0;
            }
            if ( reply_actor_index == -1 )
                goto next_row;
        }

        if ( row->required_group != -1 && protagonist_group_flags && !protagonist_group_flags[row->required_group] )
            goto next_row;

        if ( is_reply )
        {
            player_rating = 2.0f;
            priority = communication_player_speaking_priorities[priority];
        }
        else
        {
            player_rating = ai_communication_get_player_rating(protagonist_unit_index, 1u, nullptr, nullptr);
            if ( player_rating == 0.0f )
                goto next_row;
            char close_in = player_rating < 2.0f;
            int16_t comm_team = protagonist_actor_index == -1 ? -1 : actor_communication_team(protagonist_actor_index);
            if ( comm_team != -1 )
            {
                int slot = 2 * (8 * comm_team + priority) + close_in;
                if ( speech_available[slot] )
                    goto next_row;
                candidate_scratch = timer_scratch[slot];
                if ( priority < 7 )
                {
                    dialogue_event_status *event = &global_dialogue_events[2 * table_row_index + comm_team];
                    if ( event->last_time_spoken != -1 )
                    {
                        int age = event_time - event->last_time_spoken;
                        repeat_factor = (float)age * 0.0011111111f;
                        if ( repeat_factor >= 0.0f )
                        {
                            if ( repeat_factor > 1.0f )
                                repeat_factor = 1.0f;
                        }
                        else
                        {
                            repeat_factor = 0.0f;
                        }
                    }
                    int disable_until = event->disable_until_time;
                    if ( disable_until != -1 )
                    {
                        int remaining = disable_until - event_time;
                        if ( close_in )
                            remaining += 30;
                        if ( remaining > 0 )
                            goto next_row;
                    }
                }
            }
        }

        {
            int play_delay = (int)(communication_play_delays[row->protagonist_type] * 30.0f);
            int16_t ai_notification_delay = play_delay;
            if ( subject_race == _race_player && !is_reply )
                ai_notification_delay = play_delay + 30;

            int16_t row_flags = row->flags;
            float notify_delay_f;
            if ( (row->flags & (1u << _dialogue_usage_immediate_notify_bit)) != 0 )
                notify_delay_f = 0.0f;
            else
                notify_delay_f = communication_notification_delays[priority] * 30.0f;
            int notify_delay = (int)notify_delay_f;

            int recipient_look_direction = (uint16_t)row->recipient_look_direction;
            int16_t delay_time = ai_notification_delay + candidate_scratch;
            int16_t ai_delay_time = notify_delay + candidate_scratch;

            if ( recipient_look_direction >= _comm_look_direction_subject
              && recipient_look_direction <= _comm_look_direction_danger )
            {
                switch ( recipient_look_direction )
                {
                    case _comm_look_direction_subject:
                        if ( subject_unit_index != -1 ) { look_target_type = _ai_information_look_unit; look_unit_index = subject_unit_index; }
                        break;
                    case _comm_look_direction_protagonist:
                        if ( protagonist_unit_index != -1 ) { look_target_type = _ai_information_look_unit; look_unit_index = protagonist_unit_index; }
                        break;
                    case _comm_look_direction_target:
                        if ( recipient_unit_index != -1 ) { look_target_type = _ai_information_look_unit; look_unit_index = recipient_unit_index; }
                        break;
                    default:
                        if ( subject_actor_index != -1 )
                        {
                            actor_datum *actor = DATUM_GET(actor_data, actor_datum, subject_actor_index);
                            if ( actor->danger_zone.danger_type > actor_danger_zone_none )
                            {
                                look_unit_index = actor->danger_zone.object_index;  /* DEVIATION: decompiler read position.x (0x2B0); binary loads 0x28C = object_index (lwz r15, 0x28C(r11) @0x837D110C) */
                                look_target_type = _ai_information_look_object;
                            }
                        }
                        break;
                }
            }
            if ( look_target_type > 0 )
            {
                recipient_look_priority = row->recipient_look_priority;
                if ( recipient_look_priority == -1 || recipient_look_priority == 1 )
                    recipient_look_priority = communication_recipient_default_look_priorities[priority];
            }
            int16_t protagonist_look_priority = row->protagonist_look_priority;
            if ( protagonist_look_priority == -1 || protagonist_look_priority == 1 )
                protagonist_look_priority = communication_protagonist_default_look_priorities[priority];

            int16_t vocalization_type = row->vocalization_type;
            int16_t animation_type = row->animation_type;
            float consider_weight = 1.0f;   /* v205 */
            int sound_definition_index = -1;
            float animation_weight = 1.0f;  /* v134 */
            int16_t resolved_vocalization = vocalization_type;
            int16_t considered_vocalization = -1;  /* v199 */

            if ( !is_reply )
            {
                considered_vocalization = ai_communication_consider_speech(
                    protagonist_unit_index, priority, speech_priority, delay_time, row_flags & 1, 0,
                    &resolved_vocalization, &consider_weight, &sound_definition_index, nullptr);
                if ( !considered_vocalization )
                    goto next_row;
                if ( animation_type != -1 )
                {
                    uint8_t impulse_ok = unit_test_animation_impulse(protagonist_unit_index, animation_type);
                    if ( impulse_ok && protagonist_actor_index != -1 )
                    {
                        if ( actor_action_class(protagonist_actor_index) == 2 )
                            impulse_ok = 0;
                        else
                            impulse_ok &= (protagonist_actor->state.mode == _actor_mode_asleep) - 1;
                    }
                    if ( impulse_ok )
                        animation_weight = 2.0f;
                }
            }

            float weight = row->weight * animation_weight * consider_weight * player_rating
                         * reply_rating * repeat_factor;
            if ( weight > 0.0f )
            {
                if ( candidate_count >= 16 )
                    break;
                ai_communication_candidate *rec = &candidates[candidate_count];
                rec->weight = weight;
                rec->interrupts = (row_flags & 2) != 0;
                rec->is_reply = is_reply;
                rec->vocalization_type = resolved_vocalization;
                rec->priority = speech_priority;
                rec->animation_type = row->animation_type;
                rec->considered_vocalization = considered_vocalization;
                rec->delay_time = delay_time;
                rec->ai_delay_time = ai_delay_time;
                rec->protagonist_unit_index = protagonist_unit_index;
                rec->protagonist_actor_index = protagonist_actor_index;
                rec->recipient_unit_index = recipient_unit_index;
                rec->reply_actor_index = reply_actor_index;
                rec->protagonist_look_priority = protagonist_look_priority;
                rec->recipient_look_priority = recipient_look_priority;
                rec->look_target_type = look_target_type;
                rec->look_unit_index = look_unit_index;
                rec->sound_definition_index = sound_definition_index;
                rec->table_row_index = table_row_index;
                if ( (row_flags & 2) != 0 )   /* bit1 = interrupts (see rec->interrupts above); row flags word has no DB enum */
                    any_interrupt = 1;
                total_weight += weight;
                ++candidate_count;
            }
        }

next_row:
        ++row;
        ++table_row_index;
    }
    while ( row->communication_type == communication_type );

    /* ---- select and execute one candidate ---- */
    if ( candidate_count <= 0 )
        return;

    ai_communication_candidate *chosen = candidates;
    if ( any_interrupt )
    {
        total_weight = 0.0f;
        for ( int i = 0; i < candidate_count; i = (int16_t)(i + 1) )
        {
            ai_communication_candidate *rec = &candidates[i];
            if ( !rec->interrupts )                 /* not flagged-interrupt: drop weight to 0; rec[4]->interrupts (off 0x04) */
                *(float *)rec = 0.0f;
            total_weight += *(float *)rec;
        }
    }

    if ( candidate_count > 1 )
    {
        float accumulated = 0.0f;
        unsigned int *seed = get_global_random_seed_address();
        float threshold = real_seed_random(seed) * total_weight;
        int16_t selected = 0;
        int16_t index = 0;
        do
        {
            accumulated += candidates[index].weight;
            if ( accumulated >= threshold )
                break;
            selected = ++index;
        }
        while ( index < candidate_count - 1 );
        chosen = &candidates[selected];
    }

    int target_unit_index = chosen->recipient_unit_index;
    int16_t look_type = chosen->look_target_type;
    int look_unit = chosen->look_unit_index;
    int16_t dialogue_type_index = chosen->table_row_index;

    ai_information_packet packet;
    packet.look_priority = chosen->recipient_look_priority;
    packet.updated_dialogue_timers = 1;
    packet.communication_type = communication_type;
    packet.dialogue_type_index = dialogue_type_index;
    packet.look_type = look_type;
    packet.damage_category = damage_index;
    packet.look_data.___u0.unit.unit_index = look_unit;   /* recovered: *(int *)packet.look_data.raw -> look_data.___u0.unit.unit_index */
    packet.target_unit_index = target_unit_index;
    packet.information_type = information_type == -1 ? 0 : information_type;
    if ( information_data )
        packet.information_data = *information_data;
    else
        for ( unsigned i = 0; i < sizeof(packet.information_data); ++i )
            ((unsigned char *)&packet.information_data)[i] = 0;  /* byte-wise clear; DB union has no raw view */

    if ( chosen->is_reply )
    {
        int16_t priority = chosen->priority;
        int unit_index = chosen->protagonist_unit_index;
        int16_t vocalization_type = chosen->vocalization_type;
        ai_communication_notify(unit_index, priority, vocalization_type, &packet);
        ai_communication_finished(unit_index, priority, vocalization_type, 1u, chosen->reply_actor_index, &packet);
        return;
    }

    /* speak path */
    int16_t priority = chosen->priority;
    int sound_definition_index = chosen->sound_definition_index;
    int16_t delay_time = chosen->delay_time;
    int16_t ai_notification_delay = chosen->ai_delay_time;

    unit_speech_item speech;
    speech.priority = priority;
    speech.vocalization_type = chosen->vocalization_type;
    speech.sound_definition_index = sound_definition_index;
    speech.delay_time = delay_time;
    speech.ai_notification_delay = ai_notification_delay;
    speech.pause_time = 24;
    /* copy the 8 ai-packet dwords (target/look/etc.) from the chosen record into speech.ai */
    speech.ai = packet;

    int speaker_unit = chosen->protagonist_unit_index;
    unit_speak(speaker_unit, chosen->considered_vocalization, &speech);

    int16_t animation_impulse = (uint16_t)chosen->animation_type;
    if ( (uint16_t)animation_impulse != 0xFFFF )   /* DEVIATION: the (uint16_t) load cast was undone by the int16_t local, so the promoted -1 made this always TRUE and unit_start_animation_impulse ran with -1; binary zero-extends, lhz r27,0xA(r31) @0x837D15F4 + cmplwi cr6,r27,0xFFFF @0x837D15F8 */
    {
        unit_datum *speaker_object = (unit_datum *)object_from_index(speaker_unit);
        real_vector2d facing = *(real_vector2d *)&speaker_object->object.forward;
        if ( target_unit_index != -1 )
        {
            real_point3d speaker_head, target_head;
            unit_get_head_position(speaker_unit, &speaker_head);
            unit_get_head_position(target_unit_index, &target_head);
            facing.n[0] = target_head.n[0] - speaker_head.n[0];
            facing.n[1] = target_head.n[1] - speaker_head.n[1];
            float length = __fsqrts(facing.n[1] * facing.n[1] + facing.n[0] * facing.n[0]);
            if ( __fabs(length) < 0.000099999997f
              || (facing.n[0] = (1.0f / length) * (target_head.n[0] - speaker_head.n[0]),
                  facing.n[1] = (target_head.n[1] - speaker_head.n[1]) * (1.0f / length),
                  length == 0.0f) )
            {
                facing = *(real_vector2d *)&speaker_object->object.forward;
            }
        }
        unit_start_animation_impulse(speaker_unit, animation_impulse, &facing);
    }

    int reply_actor = chosen->protagonist_actor_index;
    if ( reply_actor != -1 )
        ai_communication_look_secondary_at_unit(reply_actor, 9, chosen->protagonist_look_priority, target_unit_index, -1);

    /* Deviation: the decompiler's 5th arg (reply_table_index) is an uninitialised local (v169); passed 0. */
    ai_communication_update_speech_timers(speaker_unit, priority, dialogue_type_index, -1); /* phantom 5th arg dropped */
}
