/* actor_in_combat @0x8371DB90 — true when the actor is in the combat awareness state (word +53 == 3) and its
 * current combat status (word +55) outranks its previous status (word +57). */

#include <stdint.h>

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


/* return attested uint8_t: callee byte-normalizes (clrlwi r3,24 @0x8371DBD4) and callers
 * re-normalize with clrlwi 24 (encounter_update_status @0x8370D748, actor_perception_update). */
uint8_t actor_in_combat(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->state.mode != _actor_mode_combat )
        return 0;
    int16_t previous_status = actor->state.artificial_combat_status;
    int16_t status = actor->state.combat_status;
    if ( status <= previous_status )
        return 0;
    return 1;
}
