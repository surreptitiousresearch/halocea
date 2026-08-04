/* unit_dialogue_update @ 0x837BB3C0 — advance a unit's AI dialogue/vocalization state machine each tick.
 * When the "needs dialogue selection" flag is set (unit+516 bit 0x100) it picks the dialogue tag variant
 * matching the unit's actor type (unit+190), falling back to variant 0 then -1, stores it at unit+900 and
 * clears the flag. It then counts down the speech timers (unit+1000 stutter/repeat, unit+1004), and when a
 * vocalization is pending (unit+904) drives its lifecycle: at start it fires the head-marker impulse sound
 * (unit+908) and ai_communication_started; on the body timer it fires ai_communication_finished; it issues
 * ai_communication_notify once the look timer drains, clears the vocalization when fully done, and finally,
 * if a queued speech item exists (unit+952), hands it to unit_speak.
 *
 * DEVIATION: the impulse-sound is_local_player argument was misrendered by the decompiler as LOBYTE of a
 * stale float; disasm (0x837BB5D0 `li r9,0`) shows it is always 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/ai_information_packet.h"
#include "headers/unit_speech_item.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_definition.h"

extern int unit_find_dialogue_variant(const unit_definition *definition, int16_t variant_number);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);
extern void ai_communication_started(int unit_index, int16_t priority, int16_t vocalization_type, const ai_information_packet *ai_information);
extern void ai_communication_finished(int unit_index, int16_t priority, int16_t vocalization_type, uint8_t reply_to_player, int preselected_reply_actor_index, ai_information_packet *ai_information);
extern void ai_communication_notify(int unit_index, int16_t priority, int16_t vocalization_type, ai_information_packet *ai_information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);

void unit_dialogue_update(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( (unit->unit.flags & (1u << _unit_must_set_up_dialogue_bit)) != 0 )
    {
        __int16 actor_type = unit->object.variant_number;
        unit_definition *definition = TAG_GET(unit_definition, unit->definition_index);
        int dialogue_variant;
        if ( actor_type <= 0
            || (dialogue_variant = unit_find_dialogue_variant(definition, actor_type)) == -1 )
        {
            dialogue_variant = unit_find_dialogue_variant(definition, 0);
            if ( dialogue_variant == -1 )
                dialogue_variant = unit_find_dialogue_variant(definition, -1);
        }
        unit->unit.dialogue_index = dialogue_variant;
        unit->unit.flags &= ~(1u << _unit_must_set_up_dialogue_bit);
    }

    int stutter_timer = unit->unit.speech.damage_minor_decay_timer;
    if ( stutter_timer > 0 )
    {
        __int16 remaining = stutter_timer - 1;
        unit->unit.speech.damage_minor_decay_timer = remaining;
        if ( !remaining )
        {
            int repeat_count = unit->unit.speech.damage_minor_sounds;
            if ( repeat_count > 0 )
            {
                unit->unit.speech.damage_minor_sounds = repeat_count - 1;
                unit->unit.speech.damage_minor_decay_timer = 22;
            }
        }
    }

    int damage_minor_timer = unit->unit.speech.damage_minor_timer;
    if ( damage_minor_timer > 0 )
        unit->unit.speech.damage_minor_timer = damage_minor_timer - 1;
    damage_minor_timer = unit->unit.speech.damage_minor_timer;
    if ( damage_minor_timer > 0 )
        unit->unit.speech.damage_minor_timer = damage_minor_timer - 1;

    if ( unit->unit.speech.current.priority > 0 )
    {
        int delay_timer = unit->unit.speech.pre_delay_timer;
        if ( delay_timer <= 0 )
        {
            if ( !unit->unit.speech.played )
            {
                real_point3d position;
                real_vector3d forward;
                __int16 node_index;
                object_marker marker;
                if ( object_get_marker_by_name(unit_index, "head", &marker, 1) )
                {
                    node_index = marker.node_index;
                    position = *(real_point3d *)marker.node_matrix.n[3];
                    forward = *(real_vector3d *)marker.node_matrix.n[0];
                }
                else
                {
                    node_index = 0;
                    position.n[0] = global_origin3d->n[0];
                    position.n[1] = global_origin3d->n[1];
                    position.n[2] = global_origin3d->n[2];
                    forward.n[0] = global_forward3d->n[0];
                    forward.n[1] = global_forward3d->n[1];
                    forward.n[2] = global_forward3d->n[2];
                }

                int sound_definition = unit->unit.speech.current.sound_definition_index;
                if ( sound_definition != -1 )
                    unit->unit.speech.impulse_sound_index = object_impulse_sound_new(unit_index, sound_definition,
                        node_index, &position, &forward, 1.0f, 0);
                ai_communication_started(unit_index, unit->unit.speech.current.priority,
                    unit->unit.speech.current.vocalization_type, &unit->unit.speech.current.ai);
                unit->unit.speech.played = 1;
            }

            int look_timer = unit->unit.speech.ai_delay_timer;
            if ( look_timer > 0 )
                unit->unit.speech.ai_delay_timer = look_timer - 1;

            int body_timer = unit->unit.speech.sound_timer;
            if ( body_timer <= 0 )
            {
                if ( !unit->unit.speech.finished )
                {
                    ai_communication_finished(unit_index, unit->unit.speech.current.priority,
                        unit->unit.speech.current.vocalization_type,
                        0, -1, &unit->unit.speech.current.ai);
                    unit->unit.speech.finished = 1;
                }
                int tail_timer = unit->unit.speech.post_delay_timer;
                if ( tail_timer > 0 )
                    unit->unit.speech.post_delay_timer = tail_timer - 1;
                if ( !unit->unit.speech.post_delay_timer )
                    unit->unit.speech.ai_delay_timer = 0;
            }
            else
            {
                __int16 remaining = body_timer - 1;
                unit->unit.speech.sound_timer = remaining;
                if ( !remaining )
                    unit->unit.speech.impulse_sound_index = -1;
            }
        }
        else
        {
            unit->unit.speech.pre_delay_timer = delay_timer - 1;
        }
    }

    if ( !unit->unit.speech.ai_delay_timer && !unit->unit.speech.notified_ai )
    {
        ai_communication_notify(unit_index, unit->unit.speech.current.priority,
            unit->unit.speech.current.vocalization_type, &unit->unit.speech.current.ai);
        unit->unit.speech.notified_ai = 1;
    }

    if ( unit->unit.speech.current.priority > 0 && !unit->unit.speech.sound_timer && !unit->unit.speech.post_delay_timer )
        unit->unit.speech.current.priority = 0;

    if ( !unit->unit.speech.current.priority && unit->unit.speech.queued.priority > 0 )
        unit_speak(unit_index, 3, &unit->unit.speech.queued);
}
