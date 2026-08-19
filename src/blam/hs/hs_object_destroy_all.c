/* hs_object_destroy_all @0x837F7DC8 — delete every object in the world except players and anything attached
 * to a player. First it ejects every player riding a vehicle, then it walks all top-level objects and, for
 * each, determines whether it is "protected" (it is a player unit, has a player somewhere in its child
 * hierarchy, has a player up its parent/mount chain, or is an item that belongs to a player) and deletes
 * only the unprotected ones. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/object_iterator.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_datum.h"
#include "headers/item_flags.h"
#include "headers/player_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int object_get_ultimate_parent(int object_index);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern int player_index_from_unit_index(int unit_index);
extern uint8_t object_is_or_contains_player(int object_index);
extern void object_delete(int object_index);

void hs_object_destroy_all(void)
{
    /* Eject any player currently riding another object. */
    data_iterator player_iterator;
    data_iterator_new(&player_iterator, player_data);
    for ( player_datum *player = data_iterator_next(&player_iterator); player;
          player = data_iterator_next(&player_iterator) )
    {
        if ( player->unit_index != -1 )
        {
            int unit_index = player->unit_index;
            if ( object_get_ultimate_parent(unit_index) != unit_index )
                unit_exit_seat_end(unit_index, 0, 1u, 1u);
        }
    }

    /* Delete every unprotected top-level object. */
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_all, 0);
    for ( object_datum *object = object_iterator_next(&iterator); object; object = object_iterator_next(&iterator) )
    {
        if ( object->object.parent_object_index != -1 )
            continue;

        object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, iterator.index)->datum;
        int protected_object = player_index_from_unit_index(iterator.index) != -1;

        if ( !protected_object )
        {
            int child = object_data->object.first_child_object_index;
            if ( child != -1 )
            {
                while ( 1 )
                {
                    object_datum *child_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum;
                    if ( object_is_or_contains_player(child) )
                        break;
                    child = child_data->object.next_object_index;
                    if ( child == -1 )
                        goto check_mount_chain;
                }
                protected_object = 1;
            }
        }

check_mount_chain:
        if ( !protected_object )
        {
            int mount = object_data->object.parent_object_index;
            if ( mount != -1 )
            {
                while ( 1 )
                {
                    object_datum *mount_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, mount)->datum;
                    if ( player_index_from_unit_index(mount) != -1 )
                        break;
                    mount = mount_data->object.parent_object_index;
                    if ( mount == -1 )
                        goto check_type;
                }
                protected_object = 1;
            }
        }

check_type:
        /* item objects (weapon/equipment/garbage) that belong to a player are protected */
        if ( !protected_object
          && ((1 << object_data->object.type) & object_mask_item) != 0
          && (((item_datum *)object_data)->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
            protected_object = 1;

        if ( !protected_object )
            object_delete(iterator.index);
    }
}
