/* ?OnChangeMode@HALO_MNG@@UAAXKK@Z @0x823D06B0 */
#include "../headers/hcex/HALO_MNG.h"
#include "../headers/hcex/HALO_MNG_boundary.h"

// 0x823D06C4 — chain to the base, then start/stop the Blam main-loop clock on a "paused" (bit
// 0x2) mode transition: pausing (bit newly clear) restarts time via main_start_time(); un-pausing
// (bit newly set) halts it via main_stop_time(). (Naming: "paused" is this batch's interpretation
// of mode bit 0x2 based on the start/stop-time behavior; the bit itself isn't named in the DB.)
void HALO_MNG::OnChangeMode(unsigned int oldMode, unsigned int newMode)
{
    iaIACTOR::OnChangeMode(oldMode, newMode);

    if ( (oldMode & 2) != 0 )
    {
        if ( (newMode & 2) == 0 )
            main_start_time();
    }
    else if ( (newMode & 2) != 0 )
    {
        main_stop_time();
    }
}
