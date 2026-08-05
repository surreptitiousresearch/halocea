/* actor_perception_find_recent_damaging_prop_index @0x837D6AA8 — the actor's own recent-damage-history
 * counterpart of actor_perception_find_killer_prop_index.c: looks at the actor's own damage history
 * (actor +24 -> object, then object+1072, four 16-byte {damage_amount, ..., damager_object_index@+8}
 * records) and returns the actor's perception prop for whichever damager dealt the most damage, provided
 * that damager resolves to a known unit, has a prop the actor currently perceives, that prop is in an
 * active perception state (2..3), and — unless skip_friendlies is set — the prop is an enemy. Returns the
 * prop index, or -1 if none qualifies. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_attacker.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern int ai_get_responsible_unit(int object_index, uint8_t responsible_for_weapon_fire);
extern int prop_get_active_by_unit_index(uint16_t actor_index, int unit_index);

int actor_perception_find_recent_damaging_prop_index(int actor_index, uint8_t skip_friendlies)
{
    int best_prop_index = -1;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int object_index = actor->meta.unit_index;
    if ( object_index == -1 )
        return best_prop_index;

    unsigned int best_damage = 0;
    /* DEVIATION: prior source cast datum to int* and added 1072 with int-pointer arithmetic (+4288 bytes);
     * decompiler adds 1072 BYTES to the datum pointer = unit_datum.unit.attackers (unit_attacker[4] @1072).
     * Typed container restores that. NOTE: the record field ranked by is @0 = game_time_stamp (not
     * damage_inflicted @4), so "best" is the most-recent attacker; damager object index is @8. */
    unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    unit_attacker *attackers = object->unit.attackers;

    for ( int i = 0; i < 4; i++ )
    {
        unit_attacker *attacker = &attackers[i];
        int responsible_unit = ai_get_responsible_unit(attacker->object_index, 1u);
        if ( responsible_unit == -1 )
            continue;

        int prop_index = prop_get_active_by_unit_index(actor_index, responsible_unit);
        if ( prop_index == -1 )
            continue;

        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int16_t perception_state = prop->state;
        if ( perception_state >= _prop_state_becoming_unacknowledged && perception_state <= _prop_state_acknowledged
          && (prop->enemy || !skip_friendlies)
          && attacker->game_time_stamp > best_damage )
        {
            best_prop_index = prop_index;
            best_damage = attacker->game_time_stamp;
        }
    }

    return best_prop_index;
}
