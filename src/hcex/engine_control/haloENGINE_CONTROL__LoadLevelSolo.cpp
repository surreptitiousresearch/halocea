#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::LoadLevelSolo @ 0x823C7008 — load `map` in solo mode at `difficulty`,
// optionally starting a new save (deferring the map change until the next frame when this is the
// very first solo load).
// CAVEAT: dsTSTRING construction/teardown reproduced per this corpus's established convention (see
// LoadMainMenu.cpp) — the decompiler's per-line apPROCESSOR_LOCK checks are that method's own
// inlined body, not reproduced at each call site. `map` is passed by value (its own ref is dropped
// on every path, matching the decompiler's final refcount-- on the incoming argument).
void haloENGINE_CONTROL::LoadLevelSolo(dsTSTRING<char> map, short difficulty, bool isNew)
{
    if (load_as_halo)
    {
        isCoop = 0;

        char mapName[272];
        fnmGetName(map.pBuffer->str, mapName);

        char levelPath[260];
        sprintf_0(levelPath, "levels\\%s\\%s", mapName, mapName);

        dsTSTRING<char> level;
        level.pBuffer = nullptr;
        level.UnsafeInit(mapName, -1, 0);
        curLevel = level;
        if (--level.pBuffer->refCount == 0)
            dlFree(level.pBuffer);

        dsTSTRING<char> emptyCheckpoint;
        emptyCheckpoint.pBuffer = nullptr;
        emptyCheckpoint.UnsafeInit("", -1, 0);
        curCheckpoint = emptyCheckpoint;
        if (--emptyCheckpoint.pBuffer->refCount == 0)
            dlFree(emptyCheckpoint.pBuffer);

        main_set_difficulty(difficulty);
        main_set_map_name(levelPath);

        if (isNew)
        {
            main_defer_map_map_change();
            if (!firstLoadInSolo)
                ui_widget_event_handler_function_invoke(nullptr, nullptr, 73, nullptr);
        }

        hcex_call_frame = 0;
        player_spawn_count = 1;
        hcex_split_screen = 0;

        if (dbg_hcex_test_online_coop_local.value)
            hcex_coop_online_test_local = 1;

        if (hcex_coop_online_test_local)
        {
            player_spawn_count = 2;
            hcex_coop_local_player_index = 1;
        }
    }

    if (--map.pBuffer->refCount == 0)
        dlFree(map.pBuffer);
}
