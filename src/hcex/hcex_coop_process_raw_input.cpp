/* hcex_coop_process_raw_input @0x823BB240 — per-tick coop gate: if either local or remote
 * player's raw pad state is holding Back+Start together, and the running cinematic currently
 * allows it, mark the shared action envelope so the next deterministic-data pass skips the
 * cinematic (see hcex_coop_process_deteministic_data -> main_skip_cinematic).
 * cinematic_can_be_skipped is a Blam boundary (already reversed elsewhere). */

#include <stdint.h>
#include "../headers/hcex/haloRAW_INPUT_X360.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

extern int  hcex_raw_input_back_start(unsigned int buttons);
extern "C" uint8_t cinematic_can_be_skipped(void);

void hcex_coop_process_raw_input(const haloRAW_INPUT_X360 *local_input,
                                  const haloRAW_INPUT_X360 *remote_input,
                                  haloPLAYER_ACTIONS_ENV *out_actions)
{
    out_actions->skip_cine = 0;

    if ( hcex_raw_input_back_start(local_input->buttons) ||
         hcex_raw_input_back_start(remote_input->buttons) )
    {
        if ( cinematic_can_be_skipped() )
            out_actions->skip_cine = 1;
    }
}
