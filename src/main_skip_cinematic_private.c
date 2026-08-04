/* main_skip_cinematic_private @0x8368A128 — perform a queued cinematic skip if the cinematic permits it:
 * fire the stop event and queue a checkpoint revert; always clears the skip request. */

#include <stdint.h>
#include "headers/main_globals.h"

extern uint8_t cinematic_can_be_skipped(void);
extern void hcex_fire_event(const char *event_name);

void main_skip_cinematic_private(void)
{
    if ( cinematic_can_be_skipped() )
    {
        hcex_fire_event("cinameticStop");
        main_globals.switch_to_structure_bsp_index = -1;
        main_globals.save_map = 0;
        main_globals.revert_map = 1;
        main_globals.lost_map = 0;
    }
    main_globals.skip_cinematic = 0;
}
