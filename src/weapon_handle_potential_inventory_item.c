/* weapon_handle_potential_inventory_item @0x836D89F0 */
/* weapon_handle_potential_inventory_item 0x836D89F0 — try to absorb a walked-over item's ammunition into a
 * weapon's magazines. For each weapon magazine that is not full, either (a) merge rounds directly from a
 * dropped weapon of the same type (transferring loaded rounds, playing the pickup sound, deleting the source
 * if emptied, and notifying the network if mid-reload), or (b) consume a matching ammunition pack listed in
 * the magazine's ammunition-type table (running the equipment pickup and deleting the item). The number of
 * rounds taken for the last handled magazine is returned through *rounds_picked_up. Returns 1 if any item was
 * consumed, else 0.
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_ammunition_object.h"
#include "headers/weapon_magazine_state.h"
#include "headers/blam_data_globals.h"


extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern void object_delete(int object_index);
extern void weapon_ammo_pickup_mid_reload_to_network(int weapon_index, int16_t magazine_index, int16_t rounds_picked_up);
extern void equipment_definition_handle_pickup(int equipment_definition_index);

uint8_t weapon_handle_potential_inventory_item(int weapon_index, int item_index, int16_t local_player_parent_index,
        int16_t *rounds_picked_up)
{
    int result = 0;
    int magazine_index = 0;

    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_datum *item = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    int item_definition_index = item->definition_index;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    if ( definition->weapon.magazines.count <= 0 )
        return result;

    while ( 1 )
    {
        weapon_magazine_definition *magazine_def =
            &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine_index];
        int magazine_capacity = magazine_def->rounds_total_maximum;
        int magazine_current = weapon->weapon.magazines[magazine_index].rounds_total;
        if ( magazine_current < magazine_capacity )
        {
            int16_t rounds_to_pick = 0;
            int16_t rounds_needed = magazine_capacity - magazine_current;

            if ( weapon->definition_index == item_definition_index )
            {
                /* same weapon type — transfer its loaded rounds */
                int16_t transfer = magazine_capacity - magazine_current;
                weapon_magazine *item_magazine = &item->weapon.magazines[magazine_index];
                int16_t item_loaded = item_magazine->rounds_total;
                if ( item_loaded <= rounds_needed )
                    transfer = item_magazine->rounds_total;
                rounds_to_pick = transfer;
                if ( transfer > 0 )
                {
                    item_magazine->rounds_total = item_loaded - transfer;
                    if ( definition->weapon.pickup_sound.index != -1 && local_player_parent_index != -1 )
                        unspatialized_impulse_sound_new(definition->weapon.pickup_sound.index, 1.0f);
                    if ( !item_magazine->rounds_total )
                        object_delete(item_index);
                    if ( !weapon->object.datum_role
                      && weapon->weapon.magazines[magazine_index].state == _weapon_magazine_reloading )
                        weapon_ammo_pickup_mid_reload_to_network(weapon_index, (int16_t)(magazine_index + 1),
                            rounds_to_pick);
                }
            }
            else
            {
                /* different item — search this magazine's ammunition-type table */
                int ammo_type_count = magazine_def->ammunition_objects.count;
                int ammo_type_item;
                if ( ammo_type_count <= 0 )
                    goto next_magazine;

                weapon_ammunition_object *ammo_types =
                    (weapon_ammunition_object *)magazine_def->ammunition_objects.address;
                int ammo_type_index = 0;
                while ( 1 )
                {
                    weapon_ammunition_object *ammo_type = &ammo_types[ammo_type_index];
                    ammo_type_item = ammo_type->object.index;
                    if ( ammo_type_item == item_definition_index )
                    {
                        int16_t rounds_per_pickup = ammo_type->rounds;
                        rounds_to_pick = rounds_needed;
                        if ( rounds_per_pickup <= rounds_needed )
                            rounds_to_pick = rounds_per_pickup;
                        if ( rounds_to_pick > 0 )
                            break;
                    }
                    ammo_type_index = (int16_t)(ammo_type_index + 1);
                    if ( ammo_type_index >= ammo_type_count )
                        goto next_magazine;
                }
                if ( local_player_parent_index != -1 )
                    equipment_definition_handle_pickup(ammo_type_item);
                object_delete(item_index);
            }

            result = 1;

next_magazine:
            weapon->weapon.magazines[magazine_index].rounds_total += rounds_to_pick;
            *rounds_picked_up = rounds_to_pick;
        }
        magazine_index = (int16_t)(magazine_index + 1);
        if ( magazine_index >= definition->weapon.magazines.count )
            return result;
    }
}
