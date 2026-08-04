/* ai_communication_actor_talk_weight @0x837CDF78 — score how appropriate it is for one actor to voice a given
 * line about a subject/cause unit. Starts at a base weight of 10, gates the actor out (weight 0) if it fails any
 * of: stimulus-range proximity to subject/cause, a positive player-attention rating (flag 2), matching the
 * subject's controlling actor (flag 4), a required animation impulse, or the speech throttle
 * (ai_communication_consider_speech). When a subject/cause is supplied (v45), additionally requires that the
 * actor can perceive that prop closely enough, and boosts the weight by closeness. Returns the weight, or 0 if
 * any gate fails.
 *
 * Deviations from the decompiler:
 *  - PPC float-arg GPR-skip: stimulus_range (an FPR/float) reserves r8, so the trailing __int16 args land one
 *    GPR/stack slot later than Hex-Rays assumes. Verified via disasm register-annotation labels (arg_56 =
 *    unit_speech_priority/"speech_priority", arg_5E = vocalization_type, arg_66 = animation_type/"animation_impulse",
 *    arg_6E = flags). Hex-Rays' phantom a29/a31/a33/a35 map to unit_speech_priority/vocalization_type/
 *    animation_type/flags respectively. The real 12-arg prototype matches the sibling finders.
 *  - The body guard Hex-Rays renders as "ai_communication_priority" at 0x837CE2DC reads r21, which is actually
 *    ai_communication_type (r9, the 7th arg); ai_communication_priority is r10/r26. Corrected here.
 *  - Single-precision math widened to double by Hex-Rays is kept as float. The final
 *    `return *((float*)&v67 + 1)` is the usual single/double register-aliasing artifact: the value returned is
 *    the computed weight. Prop fields are addressed at the raw offsets the decompiler used. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/actor_position_data.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/object_header_datum.h"
#include "headers/ai_sound_volume.h"
#include "headers/actor_mode.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


#include "headers/actor_position_data.h"
extern float ai_communication_get_player_rating(int unit_index, uint8_t test_line_of_sight, int *unit_index_reference, float *distance_reference);
extern uint8_t unit_test_animation_impulse(int unit_index, int16_t animation_impulse);
extern int16_t ai_communication_consider_speech(int unit_index, int16_t communication_priority, int16_t speech_priority, int16_t delay_ticks, uint8_t allow_vocalization_lookup, uint8_t allow_recent_disabling, int16_t *vocalization_type, float *weight, int *sound_definition_index_reference, char *debugstring);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);
extern int16_t actor_get_perception_knowledge(uint16_t actor_index, int prop_index);
extern int16_t actor_visibility_at_point(int actor_index, actor_position_data *sense_position, const real_point3d *position, char lighting, int16_t line_of_sight, uint8_t use_frustum, uint8_t store_debugging_information, int16_t combat_knowledge_type);

float ai_communication_actor_talk_weight(int actor_index, int subject_unit_index, real_point3d *subject_point,
        int cause_unit_index, real_point3d *cause_point, float stimulus_range, int16_t ai_communication_type,
        int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type,
        int16_t animation_type, int16_t flags)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    char have_target = (subject_unit_index != -1 || cause_unit_index != -1);
    int actor_unit_index = actor->meta.unit_index;
    char passes = actor->state.mode >= _actor_mode_braindead && actor_unit_index != -1 && have_target;

    float weight = 10.0f;

    if ( passes )
    {
        char in_range = 1;
        if ( subject_unit_index != -1 )
        {
            float sdz = subject_point->n[2] - actor->input.position.head_position.n[2];
            float sdx = subject_point->n[0] - actor->input.position.head_position.n[0];
            float sdy = subject_point->n[1] - actor->input.position.head_position.n[1];
            if ( sdy * sdy + (sdx * sdx + sdz * sdz) >= stimulus_range * stimulus_range )
                in_range = 0;
        }
        if ( !in_range && cause_unit_index != -1 )
        {
            float cdz = cause_point->n[2] - actor->input.position.head_position.n[2];
            float cdx = cause_point->n[0] - actor->input.position.head_position.n[0];
            float cdy = cause_point->n[1] - actor->input.position.head_position.n[1];
            if ( cdy * cdy + (cdx * cdx + cdz * cdz) < stimulus_range * stimulus_range )
                in_range = 1;
        }
        passes = in_range;
    }

    if ( passes && (flags & 2) != 0 )
    {
        float player_rating = ai_communication_get_player_rating(actor_unit_index, 0, 0, 0);
        if ( player_rating == 0.0f )
            passes = 0;
        else
            weight = player_rating * 5.0f + 10.0f;
    }

    if ( passes && (flags & 4) != 0 && subject_unit_index != -1 )
    {
        object_datum *subject_object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, subject_unit_index)->datum;
        passes = actor->input.vehicle_index == subject_object->object.parent_object_index;
    }

    if ( passes )
    {
        if ( (unsigned __int16)animation_type != 0xFFFF
          && unit_test_animation_impulse(actor_unit_index, animation_type) )
            weight = weight + 5.0f;

        if ( passes && (unsigned __int16)vocalization_type != 0xFFFF )
        {
            int16_t line_vocalization_type = vocalization_type;
            int sound_definition_index = -1;
            __int16 allowed = ai_communication_consider_speech(actor_unit_index, ai_communication_priority,
                                  unit_speech_priority, 0, flags & 1, 1u, &line_vocalization_type, &weight,
                                  &sound_definition_index, 0);
            passes = allowed != 0;
        }
    }

    if ( passes && have_target )
    {
        char subject_ok = 0;
        char cause_ok = 0;

        if ( subject_unit_index != -1 )
        {
            if ( actor_unit_index == subject_unit_index )
            {
                if ( (flags & 8) != 0 )
                    subject_ok = 1;
                else
                    passes = 0;
            }
            else
            {
                int prop_index = prop_get_base_by_unit_index(actor_index, subject_unit_index, 1u, 0);
                if ( prop_index != -1 )
                {
                    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
                    if ( prop->distance <= stimulus_range )
                    {
                        int awareness = prop->state;
                        if ( (awareness >= _prop_state_becoming_unacknowledged && awareness <= _prop_state_acknowledged)
                          || (!prop->enemy
                              && (ai_communication_type
                                  || prop->audibility >= _ai_sound_volume_loud
                                  || prop->ineffability >= 2
                                  || (actor_visibility_at_point(actor_index,
                                          &actor->input.position,
                                          &prop->head_position,
                                          prop->flashlight ? 2 : prop->lighting,
                                          prop->line_of_sight, 1u, 0,
                                          actor_get_perception_knowledge(actor_index, prop_index)) >= 2))) )
                            subject_ok = 1;
                    }
                    if ( subject_ok )
                        weight = (1.0f - prop->distance / stimulus_range) * 10.0f + weight;
                }
            }
        }

        if ( cause_unit_index != -1 )
        {
            if ( actor_unit_index == cause_unit_index )
            {
                if ( (flags & 0x10) != 0 )
                    cause_ok = 1;
                else
                    passes = 0;
            }
            else
            {
                int prop_index = prop_get_active_by_unit_index(actor_index, cause_unit_index);
                if ( prop_index != -1 )
                {
                    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
                    if ( prop->distance <= stimulus_range )
                    {
                        int awareness = prop->state;
                        if ( (awareness >= _prop_state_becoming_unacknowledged && awareness <= _prop_state_acknowledged) || !prop->enemy )
                            cause_ok = 1;
                    }
                    if ( cause_ok )
                        weight = (1.0f - prop->distance / stimulus_range) * 10.0f + weight;
                }
            }
        }

        if ( passes )
            passes = cause_ok | subject_ok;
    }

    return passes ? weight : 0.0f;
}
