/* actor_clear_discarded_firing_positions @0x837EFCE8 — reset an actor's discarded firing-position cache: clears
 * the count (word 483) and the four discarded-position index slots (word 485..488), then clears the
 * has-discarded flag (byte 984) unless clear_temporary_only is set and the cached entry is not temporary
 * (byte 985). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_clear_discarded_firing_positions(int actor_index, uint8_t clear_temporary_only)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->firing_positions.next_discarded_firing_positions_entry = 0;  /* recovered: *((_WORD*)actor+483) -> +966 */
    for ( int i = 0; i < 4; ++i )
        actor->firing_positions.discarded_firing_positions[i].index = -1;
    /* bytes +984/+985 are the scalar last-discarded valid/temporary flags, not an array element */
    if ( actor->firing_positions.last_discarded_firing_position_valid
      && (!clear_temporary_only || actor->firing_positions.last_discarded_firing_position_temporary) )
        actor->firing_positions.last_discarded_firing_position_valid = 0;
}
