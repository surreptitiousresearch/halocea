/* actor_is_noncombat @0x8371DB50 — true when the actor's state.mode indicates it is not in combat.
 * Reproduces the database's branchless (sign + threshold) test verbatim; the combat threshold is the
 * DB-verified actor_mode enumerator _actor_mode_combat (== 3). */

#include <stdint.h>

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


/* return attested uint8_t: callee bool-normalizes (clrlwi r3,31 @0x8371DB84); caller
 * prop_status_refresh stores it as a byte (stb r29,0x12B @0x837DABE8). */
uint8_t actor_is_noncombat(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    /* the DB's branchless (sign + threshold) test reduces exactly to a signed compare: noncombat iff
     * state.mode < _actor_mode_combat (the 0x80000000 term was the __int16 sign check). */
    return actor->state.mode < _actor_mode_combat;
}
