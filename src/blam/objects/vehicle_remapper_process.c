/* vehicle_remapper_process @0x837013D8 — run the per-team or free-for-all vehicle substitution pass once the
 * remapper has been populated; marks the remapper processed regardless. */

#include <stdint.h>
#include "headers/vehicle_remapper_s.h"
#include "headers/game_variant.h"

extern uint8_t game_engine_running(void);
extern void vehicle_remapper_process_by_teams(void);
extern void vehicle_remapper_process_no_teams(void);

void vehicle_remapper_process(void)
{
    if ( game_engine_running() )
    {
        if ( vehicle_remapper.is_teamplay )
            vehicle_remapper_process_by_teams(); /* attested void: r3-thread removed */
        else
            vehicle_remapper_process_no_teams();
    }
    vehicle_remapper.processed = 1;
}
