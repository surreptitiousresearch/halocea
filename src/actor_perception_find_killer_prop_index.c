/* actor_perception_find_killer_prop_index @0x837D69A8 — given a prop the actor just saw killed, look at the
 * killed object's recent-damage history (4 records) and return the actor's perception prop for whichever
 * damager dealt the most damage, provided that damager resolves to a known unit, has a prop the actor
 * currently perceives, that prop is in an active perception state (2..3), and — unless skip_friendlies is
 * set — the prop is an enemy. Returns the prop index, or -1 if none qualifies.
 *
 * The killed object's recent-attacker history is unit_datum.unit.attackers (unit_attacker[4] at offset 1072).
 * NOTE: the record field the code ranks by is offset 0 = unit_attacker.game_time_stamp (NOT damage_inflicted
 * @4), so the "best" record is the most-recent attacker, not the one that dealt the most damage; the damager
 * object index is unit_attacker.object_index @8. prop_data has a 312-byte stride. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_attacker.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"

extern int ai_get_responsible_unit(int object_index, uint8_t responsible_for_weapon_fire);
extern int prop_get_active_by_unit_index(int actor_index, int unit_index);

int actor_perception_find_killer_prop_index(int actor_index, int killed_prop_index, uint8_t skip_friendlies)
{
    int best_prop_index = -1;
    unsigned int best_attack_time = 0;

    int killed_object_index = DATA_ARRAY_ELEMENT(prop_data, prop_datum, killed_prop_index)->unit_index;
    unit_datum *killed_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, killed_object_index)->datum);

    unit_attacker *attackers = killed_object->unit.attackers; /* offset 1072 */
    for (int i = 0; i < 4; i++)
    {
        unit_attacker *attacker = &attackers[i];
        unsigned int attack_time = attacker->game_time_stamp; /* field @0 the code ranks by */
        int responsible_unit = ai_get_responsible_unit(attacker->object_index, 1u);
        if (responsible_unit == -1)
            continue;

        int prop_index = prop_get_active_by_unit_index(actor_index, responsible_unit);
        if (prop_index == -1)
            continue;

        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        int16_t perception_state = prop->state;
        if (perception_state >= _prop_state_becoming_unacknowledged && perception_state <= _prop_state_acknowledged
            && (prop->enemy || !skip_friendlies)
            && attack_time > best_attack_time)
        {
            best_prop_index = prop_index;
            best_attack_time = attack_time;
        }
    }

    return best_prop_index;
}
