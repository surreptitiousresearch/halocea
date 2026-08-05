/* actor_get_weapon @0x8371DD40 — return the weapon object index the actor is currently using. Prefers the
 * weapon held by the actor's controlled secondary unit (actor +0x158, e.g. a vehicle gunner), falling back to
 * the actor's own unit (+0x18) unless the actor variant forbids it (variant flag 0x40). Returns -1 if none.
 *
 * The held weapon is resolved via unit_inventory_get_weapon using the unit's current weapon slot
 * (unit_datum +0x2F2 = unit.current_weapon_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_variant_definition_flags.h"
#include "headers/blam_data_globals.h"

extern int unit_inventory_get_weapon(int unit_index, int16_t index);

int actor_get_weapon(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = -1;

    if ( !actor->input.vehicle_gunner
      || actor->input.vehicle_index == -1
      || (result = unit_inventory_get_weapon(
              actor->input.vehicle_index,
              ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->input.vehicle_index)->datum)
                  ->unit.current_weapon_index),
          result == -1) )
    {
        if ( actor->meta.unit_index != -1
          && (*TAG_GET(uint32_t, actor->meta.variant_definition_index) & (1u << _actor_variant_definition_cannot_use_ranged_weapons_bit)) == 0 )
        {
            return unit_inventory_get_weapon(
                actor->meta.unit_index,
                ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum)
                    ->unit.current_weapon_index);
        }
    }
    return result;
}
