#include "../headers/hcex/HALO_MNG.h"
#include "../headers/hcex/HALO_MNG_boundary.h"

namespace {

// Shared by every `cheat.*` debug-var registration below (repeated 8x verbatim in the binary,
// once per cheat flag) -- registers a boolean debug variable named `name` with initial value
// `value` against the process-wide dbgVAR_MANAGER singleton.
void SetCheatDbgVar(const char *name, bool value)
{
    dsTSTRING<char> varName;
    varName.pBuffer = nullptr;
    varName.UnsafeInit(name, -1, 0);
    dbgVAR_MANAGER::GetManager()->SetDbgVar<bool, 1>(varName, value);
}

} // namespace

// 0x823DEA10 — on the ws-engine's per-frame "call frame" message (1016): clear the used-sfx list,
// mark this as an active call-frame, lock the video device, re-create the vertex buffer, and
// re-init the custom sound system for the current scene. On first load, one-time-initialize the
// developer console and seed any cheat flags as debug variables, then either finish the deferred
// map load (main_loop_init2) once this tick actually ran a call-frame, or (on every later tick)
// step the Blam main loop and sync the camera/object matrices. Always finishes by unlocking the
// video device and chaining to the base for default handling.
msgRES HALO_MNG::ProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    if ( msg == 1016 )
    {
        hcexUsedSfx.Clear();
        hcex_call_frame = true;
        vidLock.Lock("D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 2934);
        rasterizer_recreate_vbuf();

        dsTSTRING<char> sceneName;
        sceneName.pBuffer = nullptr;
        sceneName.UnsafeInit(gsSceneFullName, -1, 0);
        snd::SystemCustom->InitLevel(sceneName);

        if ( hcex_first_load )
        {
            static bool s_consoleInitialized;
            if ( !s_consoleInitialized )
            {
                main_loop_init_console();

                if ( cheat.deathless_player )
                    SetCheatDbgVar("0HCEX/cheats/deathless player", true);
                if ( cheat.infinite_ammo )
                    SetCheatDbgVar("0HCEX/cheats/infinite ammo", true);
                if ( cheat.bump_possession )
                    SetCheatDbgVar("0HCEX/cheats/bump possession", true);
                if ( cheat.super_jump )
                    SetCheatDbgVar("0HCEX/cheats/super jump", true);
                if ( cheat.reflexive_damage_effects )
                    SetCheatDbgVar("0HCEX/cheats/reflexive damage effects", true);
                if ( cheat.medusa )
                    SetCheatDbgVar("0HCEX/cheats/medusa", true);
                if ( cheat.omnipotent )
                    SetCheatDbgVar("0HCEX/cheats/kill one shot", true);
                if ( cheat.bottomless_clip )
                    SetCheatDbgVar("0HCEX/cheats/bottomless clip", true);

                s_consoleInitialized = true;
            }

            if ( hcex_call_frame )
            {
                main_loop_init2();
                hcex_first_load = false;
            }
        }
        else
        {
            main_coop_first_update();
            main_loop_update();

            // DEVIATION: the decompiler rendered these next two calls as
            // `hcex_cam_sync(v25)` (returning a phantom __int64) and
            // `hcex_obj_sync_matrices(SHIDWORD(v26), v26, v32, v31, v30, v29, v28, v27)` --
            // both are artifacts of stale/default IDA type overrides on these two unmangled
            // symbols (see HALO_MNG_boundary.h). The disassembly loads no registers before either
            // `bl`, and hcex_cam_sync's mangled-name-verified body (src/hcex/hcex_cam_sync.c) is
            // void(void); hcex_obj_sync_matrices's own mangled name demangles to void(void) too.
            // Reproduced here as the bare no-arg calls the machine code actually performs.
            hcex_cam_sync();
            hcex_obj_sync_matrices();
        }

        vidLock.Unlock("D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 2934);
    }

    return iaIACTOR::ProcessMsg(msg, pInfo, pSend);
}
