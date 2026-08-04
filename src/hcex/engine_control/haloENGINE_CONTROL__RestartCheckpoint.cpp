#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::RestartCheckpoint @ 0x823C630C — reload the current checkpoint (validating it
// first); invalidates the saved game and reverts the map when the checkpoint file is
// missing/unreadable.
void haloENGINE_CONTROL::RestartCheckpoint()
{
    if (!load_as_halo)
        return;

    if (verify_checkpoint(&curCheckpoint))
        hcex_game_state_set_saved_game_valid(1);
    else
        hcex_game_state_set_saved_game_valid(0);

    main_revert_map();
}
