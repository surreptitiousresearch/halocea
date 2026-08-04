#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::LoadLevelCoop @ 0x823C9DAC — load `map` in co-op mode at `difficulty`,
// optionally starting a new save and/or joining an existing online session (wiring up the relay/
// local-client-id and the deathless-player debug cheat for the online case).
// CAVEAT: dsTSTRING teardown reproduced per this corpus's established convention (see
// LoadMainMenu.cpp) — the decompiler's per-line apPROCESSOR_LOCK checks are dsTSTRING's own inlined
// body, not reproduced at each call site. `map` is passed by value with its own extra ref bumped
// (matching the decompiler's `++pBuffer->refCount` before forwarding to LoadLevelSolo) and dropped
// on every path.
void haloENGINE_CONTROL::LoadLevelCoop(dsTSTRING<char> map, short difficulty, bool isNew, bool isOnLine)
{
    if (load_as_halo)
    {
        dsTSTRING<char> mapForSolo;
        mapForSolo.pBuffer = map.pBuffer;
        ++mapForSolo.pBuffer->refCount;

        LoadLevelSolo(mapForSolo, difficulty, isNew);
        isCoop = 1;

        if (isOnLine)
        {
            SESSION_BASE *session = gSessionManager->m_vecSessions[0];
            net::RELAY *relay = SESSION_BASE_GetRelay(session);
            hcex_coop_local_player_index = net_RELAY_GetLocalClientId(relay);
            coop_input_initialize(session);

            cheat.deathless_player = 1;

            dsTSTRING<char> dbgVarName;
            dbgVarName.pBuffer = nullptr;
            dbgVarName.UnsafeInit("0HCEX/cheats/deathless player", -1, 0);
            bool deathlessOn = true;
            // SetDbgVar's second template param N (dbgVAR typeId) is non-deducible; bool == 1
            // (cf. HALO_MNG__ProcessMsg SetDbgVar<bool,1>).
            dbgVAR_MANAGER::GetManager()->SetDbgVar<bool, 1>(dbgVarName, deathlessOn);
            if (--dbgVarName.pBuffer->refCount == 0)
                dlFree(dbgVarName.pBuffer);
        }

        player_spawn_count = 2;
        if (!isOnLine)
            hcex_split_screen = 1;
    }

    if (--map.pBuffer->refCount == 0)
        dlFree(map.pBuffer);
}
