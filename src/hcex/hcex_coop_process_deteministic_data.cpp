/* hcex_coop_process_deteministic_data @0x823BB2C0 — hcex bridge: apply the pending cinematic
 * skip flag (if hcex_coop_process_raw_input set it this tick), then hand both coop players'
 * digested actions (u0 = local, u1 = remote) down to the Blam engine control object, which
 * forwards them to hcex_coop_apply_player_actions.
 *
 * DEVIATION: the decompiler rendered the u0/u1 -> local copy as two raw word-stepping pointer
 * loops with off-by-one-struct pointer arithmetic (`data[-1].skip_cine`, `&data->u0.desired_
 * zoom_level` walked past the end of u0 into u1). That is simply the compiler's struct-assignment
 * idiom for copying the two 32-byte haloPLAYER_ACTION_PACKER slots into two adjacent stack locals
 * — confirmed by cross-checking haloENGINE_CONTROL::SetDetAction's callee,
 * hcex_coop_apply_player_actions, which indexes its haloPLAYER_ACTION* argument as
 * actions[0]/actions[1]. Rewritten here as a `haloPLAYER_ACTION local_actions[2]` plus two plain
 * struct copies. */

#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

extern "C" void main_skip_cinematic(void);
extern haloENGINE_CONTROL *haloEngineCtrl;
extern void haloENGINE_CONTROL_SetDetAction(haloENGINE_CONTROL *self, haloPLAYER_ACTION *actions);
extern int  hcex_data_applied_num;

void hcex_coop_process_deteministic_data(haloPLAYER_ACTIONS_ENV *data)
{
    haloPLAYER_ACTION local_actions[2];

    if ( data->skip_cine )
        main_skip_cinematic();

    local_actions[0] = *(haloPLAYER_ACTION *)&data->u0;
    local_actions[1] = *(haloPLAYER_ACTION *)&data->u1;

    haloENGINE_CONTROL_SetDetAction(haloEngineCtrl, local_actions);
    ++hcex_data_applied_num;
}
