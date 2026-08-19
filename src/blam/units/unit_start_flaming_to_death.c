/* unit_start_flaming_to_death @0x836D0EC0 — put a unit into the "burning to death" state: drop its weapon,
 * mark it must-die/burning (unit flag bit 0x80, object damage_flags: clear bits 0x4/0x800 then set 0x800),
 * and — the first time only (unit.flaming_death_delay still 0) — seed a random burn timer (60..150),
 * record the attacker, and send the unit running blindly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern void unit_start_running_blindly(int unit_index);

void unit_start_flaming_to_death(int unit_index, int attacker_object_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_drop_current_weapon(unit_index, 1u);

    uint8_t already_burning = unit->unit.flaming_death_delay;
    uint16_t damage_flags = unit->object.damage_flags
        & ~((1u << _object_dead_bit) | (1u << _object_cannot_take_damage_bit));
    unit->unit.flags |= (1u << _unit_ignore_hard_pings_bit);                                  /* must-die */
    unit->object.damage_flags = damage_flags | (1u << _object_cannot_take_damage_bit);           /* burning */

    if ( !already_burning )
    {
        int burn_ticks = seed_random_range(get_global_random_seed_address(), 60, 150);
        uint8_t burn_byte = burn_ticks;
        if ( burn_ticks >= 1 )
        {
            if ( burn_ticks > 255 )
                burn_byte = 0xFF;
        }
        else
        {
            burn_byte = 1;
        }
        unit->unit.flaming_death_attacker_object_index = attacker_object_index;
        unit->unit.flaming_death_delay = burn_byte;
        unit_start_running_blindly(unit_index);
    }
}
