/* unit_flame_to_death @ 0x836CD518 — kill a unit that has burned to death. Clears the "on fire" and
 * related state bits (unit.flags / object.damage_flags), then applies the scenario's falling/environmental
 * death damage (game_globals falling_damage block, effect index at element[30]) crediting the unit's
 * flaming-death attacker as the damage owner when that object still exists, and finally sets the
 * die-act-of-god flag (damage_flags _object_die_act_of_god_bit) unless already dead (_object_dead_bit). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_globals_tag.h"
#include "headers/damage_data.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);

void unit_flame_to_death(int unit_index)
{
    int *falling_damage = global_game_globals->falling_damage.address;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int flags = unit->unit.flags;
    unit->unit.flags = flags & ~(1u << _unit_running_blindly_bit); /* dead store (overwritten below); 0xFDFFFFFF */
    int16_t damage_flags = unit->object.damage_flags & ~(1u << _object_cannot_take_damage_bit);
    unit->unit.flags = flags & ~((1u << _unit_running_blindly_bit) | (1u << _unit_ignore_hard_pings_bit)); /* 0xFDFFFF7F */
    unit->object.damage_flags = damage_flags;

    if ( falling_damage && falling_damage[30] != -1 )
    {
        object_datum *killer = object_try_and_get_and_verify_type(unit->unit.flaming_death_attacker_object_index, object_mask_all);
        damage_data damage;
        damage_data_new(&damage, falling_damage[30]);
        if ( killer )
        {
            damage.owner_player_index = killer->object.owner_player_index;
            int owner_object_index = killer->object.owner_object_index;
            if ( owner_object_index == -1 )
                owner_object_index = unit->unit.flaming_death_attacker_object_index;
            damage.owner_object_index = owner_object_index;
            damage.owner_team_index = killer->object.owner_team_index;
        }
        object_cause_damage(&damage, unit_index, -1, -1, -1, 0);
    }

    int16_t final_flags = unit->object.damage_flags;
    if ( (final_flags & (1u << _object_dead_bit)) == 0 )
        unit->object.damage_flags = final_flags | (1u << _object_die_act_of_god_bit);
}
