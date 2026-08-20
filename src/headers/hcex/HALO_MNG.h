#pragma once
#include "../ws/ia/iaIACTOR.h"
// HALO_MNG — the interactive-actor bridge object that hosts the legacy Blam engine inside the
// ws-engine message system (see CLAUDE.md's Architecture Overview: "an interactive actor object
// registered with the ws message system, ticked every frame"). Adds no data members of its own.
// DB-verified layout (types_members HALO_MNG): iaIACTOR (base)@0, size 200 -- identical to the
// base, confirming HALO_MNG is purely a behavioral override.

struct HALO_MNG : iaIACTOR {
    // 0x823D0648 — default: base-constructs iaIACTOR; vtable installation is implicit (compiler-
    // generated, not expanded as a field per this corpus's convention -- see propHALO_OBJ.h).
    HALO_MNG();

    // 0x823D07E8 — thunk to ~iaIACTOR (no owned resources of its own).
    ~HALO_MNG();

    // 0x823D06A8 — GetSuspendMask: this actor never contributes to the suspend mask. Protected
    // virtual override.
    unsigned int GetSuspendMask();

    // 0x823D06B0 — OnChangeMode: chain to the base, then start/stop the Blam main-loop clock when
    // the "paused" mode bit (0x2) transitions (see main_start_time/main_stop_time). Protected
    // virtual override.
    void OnChangeMode(unsigned int oldMode, unsigned int newMode);

    // 0x823E9F00 — ProcessFRAME: per-frame tick. On a dbg-triggered level-reload request, signals
    // "OnLevelReloadRequest" with the configured default map/checkpoint; then runs one Blam frame
    // (hcex_process_frame) and updates HCEX-side animation/cinematics. Protected virtual override.
    void ProcessFRAME();

    // 0x823DE9F0 — ProcessMsg: on the ws-engine's per-frame "call frame" message (1016), clears the
    // used-sfx list, locks the video device, re-creates the vertex buffer, re-inits the custom
    // sound system for the current scene, runs first-load console/cheat setup once, then either
    // finishes the deferred map load (main_loop_init2) or steps the Blam main loop and syncs the
    // camera/object matrices for this frame. Always chains to the base for default handling.
    // Protected virtual override.
    msgRES ProcessMsg(int msg, void *pInfo, msgADDR *pSend);

    // 0x823DE980 — ProcessTERM_LEVEL: chain to the base, then tear down per-level HCEX state
    // (first-person models, breakable surfaces, dead instances, object instances, cinematics, and
    // the custom sound system's level data). Protected virtual override.
    void ProcessTERM_LEVEL();
};
