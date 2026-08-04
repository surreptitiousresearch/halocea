/* actor_has_ranged_weapon @0x8371DE38 — true when the actor is holding a weapon and its unit object is not
 * ranged-attack-inhibited (object.damage_flags bit _object_ranged_attack_inhibited_bit, set by
 * object_set_ranged_attack_inhibited). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

extern int actor_get_weapon(uint16_t actor_index);

uint8_t actor_has_ranged_weapon(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = actor_get_weapon(actor_index) != -1;
    if ( result && actor->meta.unit_index != -1 )
    {
        if ( (((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum)
                  ->object.damage_flags
              & (1u << _object_ranged_attack_inhibited_bit)) != 0 )
            return 0;
    }
    return result;
}
