/* unit_kill_from_network @0x836D57B8 — (client) apply a "unit kill" message-delta: resolve the network
 * object index to a local unit; clear its "cannot host actor" flag's sibling bit (OR 4 into the 0x106 flags
 * word) and zero its current shield/body damage accumulators (0xE0/0xE4). If the message says to, ping the
 * death animation (unit_ping_animation, with an optional alignment vector only when the message carries
 * one). Stash the message's respawn_timer into the killer's player_datum if it resolves to a valid player,
 * mark the unit as died client-side (unit_died_client), and force the unit's state word (offset 4) to 3.
 * Finally, unless the object header's flag bit 3 (0x8) is set, drop its translated-index registration.
 *
 * DEVIATION: disasm_range(0x836D57B8,0x836D5940) resolves the decoded struct's exact 32-byte layout (see
 * headers/unit_kill_message.h) and confirms `has_alignment_vector`/`alignment_vector` are correctly threaded
 * through to unit_ping_animation's 9th param (a stack-spilled pointer, NULL when the message doesn't carry
 * one) and that `should_do_actual_damage` (its 10th, also stack-spilled) is unconditionally literal 1 on
 * this path — the raw decompile showed these as bare, unexplained locals (`v18` cast straight to a pointer,
 * `v10` with no visible assignment) because Hex-Rays doesn't reliably bind stack-spilled call arguments.
 * The unit pointer is a unit_datum (definition_index@0, _object_datum object@4, _unit_datum unit@500;
 * object_try_and_get_and_verify_type returns the object-pool datum, which for a unit is a unit_datum).
 * The field offsets are now typed: 0x106=object.damage_flags, 0xE0=object.body_vitality,
 * 0xE4=object.shield_vitality, 0x218=unit.player_index, offset-4=object.datum_role. (The earlier
 * 0xE0/0xE4 "shield/body damage" labels were a misread — they are the vitality fields, zeroed on kill.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/unit_kill_message.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_header_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);
extern void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death, uint8_t died_flying, uint8_t ignore_hard_pings, uint8_t force_hard_pings, float angle, int16_t body_part, const real_vector2d *alignment_vector, uint8_t gate);
extern void unit_died_client(int unit_index, uint8_t feigned);

void unit_kill_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    unit_kill_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int unit_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.unit_index);
    if ( unit_index == -1 )
        return;

    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( unit )
    {
        unit->object.damage_flags |= (1u << _object_dead_bit);         /* unit+0x106 = object.damage_flags */
        unit->object.body_vitality = 0.0f;      /* unit+0xE0 = object.body_vitality (was mislabeled "shield damage") */
        unit->object.shield_vitality = 0.0f;    /* unit+0xE4 = object.shield_vitality (was mislabeled "body damage") */
    }

    if ( message.should_cause_ping == 1 )
    {
        /* DB field is should_alignment_vector_be_null (inverse of has_alignment_vector); writer sets it
         * alignment_valid?0:1 — preserve: pass vector when NOT flagged null. */
        real_vector2d *alignment_vector = message.should_alignment_vector_be_null ? 0 : &message.alignment_vector;
        unit_ping_animation(unit_index, message.died, message.feign_death, message.died_flying,
                message.ignore_hard_pings, message.force_hard_pings, message.angle, message.damage_part,
                alignment_vector, 1u);
    }

    unit_datum *unit_for_owner = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( unit_for_owner )
    {
        int owner_player_index = unit_for_owner->unit.player_index; /* unit+0x218 = unit.player_index */
        if ( owner_player_index != -1 )
        {
            player_datum *owner = datum_try_and_get(player_data, owner_player_index);
            if ( owner )
                owner->respawn_timer = message.respawn_timer;
        }
    }

    unit_died_client(unit_index, message.feign_death);

    unit_datum *unit_for_state = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( unit_for_state )
        unit_for_state->object.datum_role = _networked_datum_autonomous; /* unit+0x04 = object.datum_role (client takes over the dead unit) */

    if ( (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        field_translated_index_unregister_local_index(&field_properties_object_index_definition, unit_index);
}
