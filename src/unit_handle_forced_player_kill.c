/* unit_handle_forced_player_kill @0x836D6058 — administratively kill a player's unit (e.g. an HCEX
 * achievement/debug/host action): ping and kill the unit locally, then replicate a "hard" kill to the
 * network (should_cause_ping/died=1, feign/died_flying=0, no alignment vector, damage_part=-1, respawn
 * timer taken from the player's own respawn_timer), mark the object's state (+4) detonated (3), and drop
 * its translated-index registration unless the persistent-object flag (+2 bit 0x8) is set.
 *
 * DEVIATION: the decompiler renders unit_kill_to_network's argument as a bogus `(unit_kill_network_data *)
 * 0x1010000` pointer cast; the real call passes the 32-byte struct by value (disasm-verified field by
 * field), reproduced here as an ordinary struct literal. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_kill_network_data.h"
#include "headers/field_properties_definition.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector2d.h"
#include "headers/real_vector2d.h"
typedef struct _field_properties_definition _field_properties_definition;
extern void *datum_try_and_get(const data_array *data, int index);
extern void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death, uint8_t died_flying, uint8_t ignore_hard_pings, uint8_t force_hard_pings, float angle, int16_t body_part, const real_vector2d *alignment_vector, uint8_t gate);
extern void unit_died(int unit_index, uint8_t feigned);
extern void unit_kill_to_network(unit_kill_network_data data);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);

void unit_handle_forced_player_kill(int player_index)
{
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( !player )
        return;

    int unit_index = player->unit_index;

    unit_ping_animation(unit_index, 1, 0, 0, 0, 0, 0.0f, -1, nullptr, 0);
    unit_died(unit_index, 0);

    unit_kill_network_data message_data;
    message_data.unit_index = unit_index;
    message_data.should_cause_ping = 1;
    message_data.died = 1;
    message_data.feign_death = 0;
    message_data.died_flying = 0;
    message_data.ignore_hard_pings = 0;
    message_data.force_hard_pings = 0;
    message_data.should_alignment_vector_be_null = 1;
    message_data.damage_part = -1;
    message_data.angle = 0.0f;
    message_data.respawn_timer = player->respawn_timer;
    unit_kill_to_network(message_data);

    object_header_datum *header_entry = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index);
    ((int *)header_entry->datum)[1] = 3;

    if ( (header_entry->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        field_translated_index_unregister_local_index(&field_properties_object_index_definition, unit_index);
}
