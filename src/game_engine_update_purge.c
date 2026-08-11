/* game_engine_update_purge @0x837469A8 — periodic multiplayer cleanup. Deletes dropped items that have
 * been idle for more than 900 ticks (skipping held items, networked-deleted items, and the CTF flag
 * weapon), then deletes any garbage objects that have been pending deletion for over 900 ticks. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/object_header_datum.h"
#include "headers/item_datum.h"
#include "headers/object_iterator.h"
#include "headers/networked_datum_role.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);
extern void object_delete(int object_index);

static void *purge_object_datum(int object_index) /* renamed: object_datum collides with datum typedef */
{
    return ((void *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
}

void game_engine_update_purge(void)
{
    int idle_cutoff = game_time_get() - 900;

    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_item, 0);
    while (object_iterator_next(&iterator))
    {
        item_datum *item = (item_datum *)purge_object_datum(iterator.index);
        if (item->item.last_owned_time < idle_cutoff && (item->item.flags & (1u << _item_in_unit_inventory_bit)) == 0)
        {
            int index = iterator.index;
            int should_delete = 1;
            if (object_try_and_get_and_verify_type(iterator.index, object_mask_weapon))
                should_delete = weapon_is_flag(index) == 0;
            if (should_delete && item->object.datum_role != _networked_datum_puppet && (item->item.flags & (1u << _item_part_of_respawn_system)) == 0)
                object_delete(iterator.index);
        }
    }

    object_iterator_new(&iterator, object_mask_biped, 0);
    while (object_iterator_next(&iterator))
    {
        object_datum *object = (object_datum *)purge_object_datum(iterator.index);
        if (object->object.idle_ticks > 900 && (object->object.damage_flags & (1u << _object_dead_bit)) != 0)
            object_delete(iterator.index);
    }
}
