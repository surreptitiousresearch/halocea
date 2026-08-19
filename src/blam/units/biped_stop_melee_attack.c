/* biped_stop_melee_attack @0x837AD520 — clear a biped's in-progress melee tick counter
 * (biped.player_melee_ticks, +1285). No-op if the object is not a biped. */

#include "headers/biped_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void biped_stop_melee_attack(int biped_index)
{
    biped_datum *biped = object_try_and_get_and_verify_type(biped_index, object_mask_biped);
    if (biped)
        biped->biped.player_melee_ticks = 0;
}
