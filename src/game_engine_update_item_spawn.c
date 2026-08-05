/* game_engine_update_item_spawn @0x8374CE38 — per-frame server tick that respawns multiplayer item/weapon pickups
 * from the scenario's netgame_equipment list. For each equipment entry that applies to the current game type, it
 * computes a phase-staggered respawn period and, when the game clock lands on a multiple of that period, either
 * confirms the existing pickup is still on its pad (refreshing its timer) or spawns a fresh item from the entry's
 * item_collection, then broadcasts a _message_netgame_equipment_new message delta to every loaded machine.
 *
 * The respawn period = phase_offset + duration, where phase_offset = (int)((i/count)*300) staggers entries across
 * a 300-tick window, and duration is the entry's spawn_time (seconds*30), the item_collection's default
 * (seconds*30), or 900 ticks when neither is set.
 *
 * DEVIATION: the fcfid sequences that build (float)i / (float)count and the spawn-timer truncation are restored
 * as plain casts. Tag-definition data is reached via *(T**)TAG_INSTANCE(index). The message-delta /
 * network-server callees are extern boundaries (their enum arg types are simplified to int and the named
 * constants passed as commented literals). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_equipment.h"
#include "headers/object_placement_data.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/netgame_equipment_new_message.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/item_collection_definition.h"
#include "headers/networked_datum_role.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/item_flags.h"
#include "headers/equipment_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/network_message_type.h"
extern float __fsqrts(float);

/* opaque boundary types */
typedef struct network_game_server network_game_server;
typedef struct _field_properties_definition _field_properties_definition;


extern unsigned int game_engine_get_type(void);
extern uint8_t match_game_type(int game_engine_type, int count, const int16_t *game_type);
extern int game_time_get(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void object_delete(int object_index);
extern int random_item(int item_collection_index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern void object_set_garbage(int object_index, uint8_t garbage);
extern void object_type_update_baseline(int object_index);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void game_engine_update_item_spawn(void)
{
    int count = global_scenario->netgame_equipment.count;

    for (int16_t i = 0; i < global_scenario->netgame_equipment.count; i = (int16_t)(i + 1))
    {
        scenario_netgame_equipment *equipment =
            (scenario_netgame_equipment *)global_scenario->netgame_equipment.address + i;

        if (!match_game_type(game_engine_get_type(), 4, equipment->game_type))
            continue;

        int phase_offset = (int)(((float)i / (float)count) * 300.0f);

        int duration_source = equipment->spawn_time;
        if (duration_source == 0 && equipment->item_collection.index != -1)
        {
            item_collection_definition *collection_def =
                TAG_GET(item_collection_definition, equipment->item_collection.index);
            duration_source = collection_def->spawn_time;
        }
        int duration = (duration_source != 0) ? 30 * duration_source : 900;
        int period = phase_offset + duration;

        if (game_time_get() % period != 0)
            continue;

        uint8_t should_spawn = 1;
        int current_object_index = equipment->run_time_spawned_item_index;
        if (current_object_index != -1)
        {
            /* item-class object; weapon_datum carries the shared object/item layers */
            weapon_datum *object = object_try_and_get_and_verify_type(current_object_index, object_mask_item);
            if (object && (object->item.flags & (1u << _item_part_of_respawn_system)))
            {
                float dx = object->object.position.__s1.x - equipment->position.__s1.x;
                float dy = object->object.position.__s1.y - equipment->position.__s1.y;
                float dz = object->object.position.__s1.z - equipment->position.__s1.z;
                if (__fsqrts((dz * dz + dx * dx) + dy * dy) <= 0.5f || !(object->object.flags & (1u << _object_at_rest_bit)))
                {
                    /* the pickup is still sitting on its pad: keep it and refresh its respawn timer */
                    should_spawn = 0;
                    object->item.last_owned_time = game_time_get() + period - 900;
                }
                else
                {
                    object_delete(equipment->run_time_spawned_item_index);
                    equipment->run_time_spawned_item_index = -1;
                }
            }
            else
            {
                equipment->run_time_spawned_item_index = -1;
            }
        }

        if (should_spawn != 1)
            continue;

        int definition_index = random_item(equipment->item_collection.index);
        object_placement_data placement;
        object_placement_data_new(&placement, definition_index, -1);
        placement.position = equipment->position;
        vector3d_from_angle(&placement.forward, equipment->facing);

        int new_object_index = object_new_with_datum_role_control(&placement, _networked_datum_autonomous);
        if (new_object_index == -1)
            continue;

        weapon_datum *object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_object_index)->datum;
        object_set_garbage(new_object_index, 0);
        if (equipment->flags & (1u << _equipment_created_at_rest_bit))
            object->object.flags |= (1u << _object_at_rest_bit);
        object->object.datum_role = _networked_datum_master;
        object_type_update_baseline(new_object_index);
        int baseline_id = object->definition_index;

        netgame_equipment_new_message message;
        message.server_object_index =
            field_translated_index_translate_index_no_default(&field_properties_object_index_definition,
                                                              new_object_index);
        if (message.server_object_index == -1)
            message.server_object_index =
                field_translated_index_server_register_local_index(&field_properties_object_index_definition,
                                                                   new_object_index);
        message.item_definition_index = baseline_id;
        message.netgame_equipment_index = i;

        int size_in_bits = message_delta_processor_encode_stateless(
            0x2A /* _message_netgame_equipment_new */, nullptr, &message, g_message_encode_buffer, 32760);
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits,
            1, 0, 0, 3);

        object->item.last_owned_time = period + object->item.last_owned_time - 900;

        item_collection_definition *collection_def =
            TAG_GET(item_collection_definition, equipment->item_collection.index);
        if (collection_def->permutations.count == 1)
        {
            object->item.flags |= (1u << _item_part_of_respawn_system);
            equipment->run_time_spawned_item_index = new_object_index;
        }
        else
        {
            object->item.flags &= ~(1u << _item_part_of_respawn_system);
        }
    }
}
