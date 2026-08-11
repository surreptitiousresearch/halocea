// haloENGINE_CONTROL__RestartLevel @0x823C14E8
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::RestartLevel @ 0x823C14FC — abandon the current save state and reload the
// current map from scratch.
void haloENGINE_CONTROL::RestartLevel()
{
    hcex_game_state_set_saved_game_valid(0);
    main_revert_map();
}
