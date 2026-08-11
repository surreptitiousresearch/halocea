#include "headers/blam_data_globals.h"
/* trouble_is_brewing_dismissed @0x83732B98 — reset the "trouble is brewing" warning state
 * (clears the per-frame flag, disarms the timer, hides the dialog). */


void trouble_is_brewing_dismissed(void)
{
    trouble_brewed_this_frame = 0;
    trouble_is_brewing_time = -1;
    trouble_brewing_dialog_visible = 0;
}
