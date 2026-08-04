/* game_engine_update_weapons @0x83746BB0 — per-tick pass over all item-class objects (type mask 0x1C).
 * Sets each object's scale to 1.0 when item flags bit 0 is set, otherwise to the item definition's
 * default scale (clamped up from 0 to 1.0). For flag objects (CTF), if the flag is unheld
 * (parent_object_index == -1) and item flags bit 0 is clear and the weapon
 * _weapon_multiplayer_inventory_flag (bit 5) is set, the bit is cleared and the engine's weapon_drop
 * callback fires. The engine's
 * weapon_update callback is then invoked for the flag object.
 *
 * DEVIATION: the decompiler drops the callback arguments (renders weapon_update()/weapon_drop() no-arg);
 * the disassembly shows weapon_update(object_index, weapon_datum*) and weapon_drop(object_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/weapon_datum_flags.h"
#include "headers/object_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/game_engine.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern struct game_engine *game_engine;

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);

extern void update_weapon_inventory(int weapon_index);
void game_engine_update_weapons(void)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_item, 0);

    /* r3 at blr is object_iterator_next residue (tail-threaded); 0 callers consume => void */
    while (object_iterator_next(&iterator))
    {
        /* iterated objects are item-class (weapon/equipment/garbage); weapon layer only touched for flags */
        weapon_datum *object = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, iterator.index)->datum;

        if (object->item.flags & (1u << _item_in_unit_inventory_bit))
        {
            object->object.scale = 1.0f;
        }
        else
        {
            float scale = TAG_GET(weapon_definition, object->definition_index)->item.scale;
            if (scale == 0.0f)
                scale = 1.0f;
            object->object.scale = scale;
        }

        if (game_engine->weapon_update)
        {
            weapon_datum *weapon_object = object_try_and_get_and_verify_type(iterator.index, object_mask_weapon);
            if (weapon_object && weapon_is_flag(iterator.index))
            {
                /* DEVIATION: inlined copy of update_weapon_inventory@0x83746B10 collapsed to a call (dup datum re-fetch of object_header_data + identical field walk offsets 0x11C/0x1F4/0x22C confirmed byte-for-byte at disasm grade; donor's weapon_index param folds to iterator.index, r3 passthrough unclobbered through the bctrl) */
                update_weapon_inventory(iterator.index);
                game_engine->weapon_update(iterator.index, weapon_object);
            }
        }
    }
}
