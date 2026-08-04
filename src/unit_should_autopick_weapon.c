/* unit_should_autopick_weapon @0x836A96D0 — decide whether a unit should automatically pick up a weapon it
 * is standing on, rather than only being offered a manual "pick up" prompt. A weapon is auto-picked when the
 * unit can pick it up and the weapon definition is flagged for it (weapon.flags & 0x10), or when the unit is
 * currently unarmed, or — outside a running multiplayer game engine — when the unit can pick it up and is
 * carrying fewer than two weapons. Otherwise the game engine gets a final say via game_engine_force_autopickup. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t unit_get_weapon_count(int unit_index);
extern uint8_t unit_approve_weapon_pickup(int unit_index, int new_weapon_index);
extern uint8_t game_engine_running(void);
extern uint8_t game_engine_force_autopickup(int unit_index, int weapon_index);

uint8_t unit_should_autopick_weapon(int unit_index, int weapon_index)
{
    int weapon_object_definition = *(int *)object_try_and_get_and_verify_type(weapon_index, object_mask_weapon);
    weapon_definition *definition = TAG_GET(weapon_definition, weapon_object_definition);
    int weapon_count = unit_get_weapon_count(unit_index);

    unsigned __int8 should_pick_up = unit_approve_weapon_pickup(unit_index, weapon_index)
        && (definition->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) != 0;
    if (should_pick_up || weapon_count == 0)
        return 1;

    if (!game_engine_running() && unit_approve_weapon_pickup(unit_index, weapon_index) && weapon_count < 2)
        return 1;

    if (game_engine_force_autopickup(unit_index, weapon_index))
        return 1;
    return 0;
}
