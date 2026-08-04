#include <stdint.h>
#include "headers/object_type.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_is_flag(int weapon_index);

BOOL can_delete_item(int item_index)
{
    return !object_try_and_get_and_verify_type(item_index, object_mask_weapon) || weapon_is_flag(item_index) == 0;
}
