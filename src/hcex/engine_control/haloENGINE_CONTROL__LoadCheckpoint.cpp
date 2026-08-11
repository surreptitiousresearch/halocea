// haloENGINE_CONTROL__LoadCheckpoint @0x823C78A8
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::LoadCheckpoint @ 0x823C78D0 — load a named checkpoint file (queued for the
// next map revert if none is currently loaded).
void haloENGINE_CONTROL::LoadCheckpoint(const dsTSTRING<char> &checkpoint)
{
    if (!load_as_halo)
        return;

    if (hcex_load_checkpoint)
    {
        dsTSTRING<char> leafName;
        leafName.pBuffer = nullptr;
        fnmGetNameStr(&leafName, &checkpoint);

        dsTSTRING<char> withDot = leafName + ".";
        dsTSTRING<char> withExt = withDot + hcex_chpt_ext;
        if (--withDot.pBuffer->refCount == 0)
            dlFree(withDot.pBuffer);
        if (--leafName.pBuffer->refCount == 0)
            dlFree(leafName.pBuffer);

        if (verify_checkpoint(&withExt))
        {
            curCheckpoint = withExt;
            hcex_game_state_set_saved_game_valid(1);
        }
        else
        {
            hcex_game_state_set_saved_game_valid(0);
            _apLog("~HCEX,Warning~LoadCheckpoint(): failed, bad checkpoint", withExt.pBuffer->str);
        }
    }
    else
    {
        hcex_game_state_set_saved_game_valid(0);
        hcex_load_checkpoint = 1;
    }

    main_revert_map();
}
