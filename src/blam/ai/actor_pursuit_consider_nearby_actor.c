/* actor_pursuit_consider_nearby_actor @0x837F3B00 — should a nearby friendly actor join/lead this pursuit?
 * The friend (looked up from the SECOND argument) must be at searching awareness (state.combat_status in
 * [2,4)) and in a compatible action (state.action): guarding (7) or none (5) always qualify; alerted (8) only
 * when the caller is not already the coordinator; searching (6) only while the search flags word
 * (search_done/search_failed byte pair at +156) is > 0 and the byte at +164 (high byte of
 * search.pursuit_location.type, big-endian) is clear. Finally the friend's actor type must designate the
 * caller's pursuit slot as its controller.
 *
 * DEVIATION (bug class 10): the DB prototype orders the parameters (actor_index, pursuit_coordinator,
 * friend_actor_index), but disasm shows r4 (named pursuit_coordinator) is the datum index looked up in
 * actor_data and r3 (named actor_index) is the u8 pursuit-slot id compared against
 * actor_type_get_pursuit_controller; r5 is unused. DB names kept, types corrected to the registers. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t actor_type_get_pursuit_controller(int16_t actor_type);

uint8_t actor_pursuit_consider_nearby_actor(uint8_t actor_index, int pursuit_coordinator, int friend_actor_index)
{
    actor_datum *friend_actor = datum_try_and_get(actor_data, pursuit_coordinator);
    if ( !friend_actor )
        return 0;

    int16_t combat_status = friend_actor->state.combat_status;
    if ( combat_status < _actor_combat_status_investigate || combat_status >= _actor_combat_status_certain )
        return 0;

    int16_t action = friend_actor->state.action;
    if ( action != actor_action_search && action != actor_action_uncover
      && (actor_index || action != actor_action_wait)
      /* faithful: byte read at +164 = big-endian HIGH byte of search.pursuit_location.type; and a 16-bit
       * read at +156 spanning the search_done/search_failed u8 pair — exact widths preserved */
      && (action != actor_action_guard
          || ((uint8_t *)&friend_actor->state.action_data.___u0.search.pursuit_location.type)[0]
          || *(int16_t *)&friend_actor->state.action_data.___u0.search.search_done <= 0) )
    {
        return 0;
    }

    return actor_type_get_pursuit_controller(friend_actor->meta.type) != actor_index;
}
