/* hs_initialize_for_new_map @ 0x83730078 — per-map HaloScript setup */
#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

extern void hs_allocate(void);
extern uint8_t hs_scenario_postprocess(uint8_t hack);
extern void object_lists_initialize_for_new_map(void);
extern void hs_runtime_initialize_for_new_map(void);

void hs_initialize_for_new_map(void)
{
    scenario *scn = (global_scenario_index == -1) ? 0 : global_scenario;
    hs_allocate();
    /* if the scenario carries compiled script (hs_syntax_data.size != 0), post-process it. */
    if ( scn && scn->hs_syntax_data.size )
        hs_scenario_postprocess(0); /* return discarded in binary */
    object_lists_initialize_for_new_map();
    hs_runtime_initialize_for_new_map(); /* attested void: r3-thread + phantom arg dropped */
}
