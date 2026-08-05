/* damage_dealt_from_network @0x836B2720 — apply a stateless "damage dealt" message from the network:
 * decode a damage_dealt_network_data payload, resolve the translated object index back to a local one,
 * and if the object is a valid biped, accumulate the shield-damage fraction into its recent/current
 * shield-damage tracking (resetting the shield decay timer and defaulting the baseline to full (1.0) the
 * first time damage_flags bit 0x8 isn't set), fully deplete the shield if the sender flagged it depleted,
 * and ping the hit animation. Always plays the multiplayer hit sound on a successful decode.
 *
 * DEVIATION: object_try_and_get_and_verify_type returns a pointer to the object_datum's start
 * (definition_index included), not directly to its _object_datum payload — confirmed against the
 * sibling damage_dealt_to_network.c's established "+4" field-offset convention; without that correction
 * the raw indices land mid-field in the wrong _object_datum members.
 *
 * The damage_part argument to unit_ping_animation is only ever assigned (from damage_flags & (1u << _object_shield_depleted_bit)) inside
 * the shield_damage>0 branch; disasm shows the register it's passed in (r9) is left untouched on the
 * shield_damage<=0 path, i.e. the shipped code passes a stale/garbage value there. Reconstructed here as
 * a 0-initialized local — the closest defensible C equivalent of "whatever was last in r9". */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/damage_dealt_network_data.h"
#include "headers/field_properties_definition.h"
#include "headers/object_datum.h"
#include "headers/real_vector2d.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void object_deplete_shield(int object_index);
extern void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death, uint8_t died_flying, uint8_t ignore_hard_pings, uint8_t force_hard_pings, float angle, int16_t body_part, const real_vector2d *alignment_vector, uint8_t gate);
extern void play_multiplayer_hit_sound(void);

void damage_dealt_from_network(message_delta_processor_header *header)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    damage_dealt_network_data data;
    if ( !message_delta_processor_decode_stateless(&data, header) )
        return;

    int object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
        data.damaged_object_index);
    uint16_t damage_flags = 0;

    if ( object_index != -1 )
    {
        object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_unit);
        if ( object )
        {
            if ( data.shield_damage > 0.0f )
            {
                damage_flags = object->object.damage_flags;
                object->object.shield_damage_decay_timer = 0;

                if ( (damage_flags & (1u << _object_shield_depleted_bit)) == 0 )
                    object->object.current_shield_damage = 1.0f;

                float current_shield_damage = object->object.current_shield_damage;
                object->object.recent_shield_damage += data.shield_damage;

                if ( current_shield_damage > 1.0f )
                    object->object.current_shield_damage = 1.0f;
                if ( object->object.recent_shield_damage > 1.0f )
                    object->object.recent_shield_damage = 1.0f;
            }

            if ( data.shields_depleted == 1 )
                object_deplete_shield(object_index);

            unit_ping_animation(object_index, 0, 0, 0, 0, 0, 0.0f, damage_flags & (1u << _object_shield_depleted_bit), (real_vector2d *)0xFFFFFFFF, 0);
        }
    }

    play_multiplayer_hit_sound();
}
