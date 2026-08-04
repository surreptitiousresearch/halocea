#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::LoadMainMenu @ 0x823C72F4 — return to the main menu (ui.map), clearing the
// deferred-first-solo-load flag.
// CAVEAT: the decompiler's apPROCESSOR_LOCK::CheckCurrentProcessor calls around each dsTSTRING
// construction/release are that method's own inlined prologue/epilogue (per the project's
// dsTSTRING convention already established elsewhere in this corpus, e.g. apCOUNTER::apCOUNTER) —
// not reproduced at the call site here.
void haloENGINE_CONTROL::LoadMainMenu()
{
    if (!load_as_halo)
        return;

    firstLoadInSolo = 0;

    dsTSTRING<char> uiLevel;
    uiLevel.pBuffer = nullptr;
    uiLevel.UnsafeInit("ui", -1, 0);
    curLevel = uiLevel;
    if (--uiLevel.pBuffer->refCount == 0)
        dlFree(uiLevel.pBuffer);

    dsTSTRING<char> emptyCheckpoint;
    emptyCheckpoint.pBuffer = nullptr;
    emptyCheckpoint.UnsafeInit("", -1, 0);
    curCheckpoint = emptyCheckpoint;
    if (--emptyCheckpoint.pBuffer->refCount == 0)
        dlFree(emptyCheckpoint.pBuffer);

    main_goto_main_menu();
    hcex_call_frame = 0;
    player_spawn_count = 1;
    hcex_split_screen = 0;
}
