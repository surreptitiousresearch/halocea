/* game_state_call_after_load_procs @0x83684548 — after a game-state load, invoke all registered after-load
 * procs in order, then clear and collect the hcex object bridge.
 *
 * Deviation: the chained r3 return values are ABI noise (each proc is void no-arg); reproduced as a sequence. */

#include "headers/blam_data_globals.h"

extern void hcex_obj_clear(void);
extern void hcex_obj_collect(void);

#define NUMBER_OF_AFTER_LOAD_PROCS (sizeof(after_load_procs) / sizeof(after_load_procs[0]))

void game_state_call_after_load_procs(void)
{
    for ( unsigned short proc_index = 0; proc_index < NUMBER_OF_AFTER_LOAD_PROCS; ++proc_index )
        after_load_procs[proc_index]();

    hcex_obj_clear();
    hcex_obj_collect();
}
