/* sound_set_master_gain @0x8371A578 — change the global sound master gain and re-apply it to all channels.
 * No-op when unchanged. Muting (was audible, now <=0) stops all sounds and clears the active flag.
 * Un-muting from silence (was 0, now >0) marks the manager active and snaps gains in (0,1] up to 1.0.
 * Otherwise the gain is stored verbatim. update_channels is then called with the (possibly stopped) value.
 * DEVIATION: the fsel idiom is reproduced as (gain >= 0) ? 0.0 : gain.
 * DEVIATION (2026-07-31): one param, not two. `gain` is a float (f1) that also consumes its GPR shadow
 * slot r3, so there is no separate r3 int param — the earlier `channel_argument` was that float shadow
 * slot misread as an argument. Matches the DB prototype and caller extern. */

#include "headers/sound_manager_globals.h"

extern void sound_stop_all(void);
extern void update_channels(void);

void sound_set_master_gain(float gain)
{
    float master_gain = sound_manager_globals.master_gain;
    if ( gain != master_gain )
    {
        if ( master_gain <= 0.0f || gain > 0.0f )
        {
            if ( master_gain == 0.0f && gain > 0.0f && (sound_manager_globals.active = 1, gain <= 1.0f) )
                sound_manager_globals.master_gain = 1.0f;
            else
                sound_manager_globals.master_gain = gain;
        }
        else
        {sound_stop_all();
            sound_manager_globals.master_gain = (gain >= 0.0f) ? 0.0f : gain;
            sound_manager_globals.active = 0;
        }
        update_channels();
    }
}
