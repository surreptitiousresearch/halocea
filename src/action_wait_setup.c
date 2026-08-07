/* action_wait_setup @0x83822734 — zero a wait_state_data block and, unless the actor is scripted to wait
 * forever (+352), populate it: carry over `actively_searching`, snapshot whether this actor is the
 * encounter's wait coordinator (+460, the same opaque flag byte documented in action_wait_control.c),
 * start the look/query timers, and request a move. Returns whether setup actually happened (false if the
 * actor waits forever).
 *
 * DEVIATION: the decompiler rendered the leading zero-init as a raw backwards-pointer loop
 * (`&state_data[-1].coordinator_prop_index`, six `*++p = 0` steps); this walks exactly the 24 bytes of
 * wait_state_data starting at its own base, i.e. a plain zero-fill — restored as a memset. */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/wait_state_data.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

uint8_t action_wait_setup(uint16_t actor_index, uint8_t actively_searching, wait_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* recovered: ((int *)state_data)[i]=0 loop -> memset (plain 24-byte zero-fill, matches sibling setups) */
    memset(state_data, 0, sizeof(wait_state_data));

    if ( actor->input.vehicle_passenger )
        return 0;

    state_data->was_actively_searching = actively_searching;
    state_data->waiting_as_coordinator = actor->external_orders.pursuit_is_coordinator;
    state_data->entry_time = game_time_get();
    state_data->look_timer = 120;
    state_data->exit_timer = 0;
    state_data->desire_move = 1;
    state_data->query_timer = seed_random_range(get_global_random_seed_address(), 300, 600);
    return 1;
}
