/* actor_communication_team @0x837CC568 — map an actor to its dialogue "communication team": 0 for the human
 * race bit (0x2 = _race_human), 1 for the covenant race bit (0x4 = _race_covenant), otherwise -1.
 * (disasm confirms rlwinm masks 0x2 then 0x4 — see actor_race.h.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_race.h"
#include "headers/blam_data_globals.h"

extern uint16_t actor_type_get_race(int16_t actor_type);

int16_t actor_communication_team(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t race = actor_type_get_race(actor->meta.type);
    if ( (race & _race_human) != 0 )
        return 0;
    if ( (race & _race_covenant) != 0 )
        return 1;
    return -1;
}
