#pragma once
/* haloENGINE_CONTROL — the C++ control object that wraps Blam engine state for the outer ws-engine
 * to query (the global `haloEngineCtrl`); per CLAUDE.md's Blam Bridge table, the single most
 * important handle the outer engine holds onto the legacy Blam engine. DB-verified layout
 * (types_members haloENGINE_CONTROL):
 *   eventDispatcher   @ 0x00 (8 bytes, EVENT_DISP_haloENGINE_CONTROL)
 *   sslObject         @ 0x08 (sslOBJ_REF, 4 bytes)
 *   curCheckpoint     @ 0x0C (dsTSTRING<char>)
 *   curLevel          @ 0x10 (dsTSTRING<char>)
 *   pathToCheckpoints @ 0x14 (dsTSTRING<char>)
 *   isCoop            @ 0x18 (bool)                                          — 25 bytes total.
 *
 * Consumers are the ws-engine C++ side: this class's own 32 reversed methods in
 * src/hcex/engine_control/, and the sibling SSL_EXPORTER_haloENGINE_CONTROL pass, which binds real
 * pointers-to-member via `&haloENGINE_CONTROL::SetMode` and needs the actual dsTSTRING<char> C++
 * template types.
 *
 * DEVIATION (2026-08-07, odr_dup drain): this header used to carry a second, plain-C `#else` view
 * that restated dsEVENT_HANDLER, EVENT_DISP_haloENGINE_CONTROL, sslOBJ_REF and haloENGINE_CONTROL
 * as flat C structs, for "legacy Blam-side bridge helpers (src/hcex/*.c, plain C)". That branch was
 * dead and unbuildable: the include graph reaches this header from 0 C TUs and 130 C++ TUs, and the
 * branch's own first include (hcex_ds_boundary.h) uses `extern "C"` and `dsTSTRING<char>`, so it
 * could never have compiled as C. Its two duplicate bodies were the only file-scope definitions of
 * dsEVENT_HANDLER and sslOBJ_REF outside their canonical headers (ws/ds/dsEVENT_HANDLER.h,
 * ws/ssl/sslOBJ_REF.h) — both of which this header already includes below, layout-identical and
 * DB-verified (dsEVENT_HANDLER: __vftable@0, size 4; sslOBJ_REF: pObject@0, size 4). Removed; the
 * C++ view is now the only view, so a C TU that reaches here fails loudly instead of silently
 * seeing a different type. */

#include "../ws/ds/dsTSTRING.h"
#include "../ws/ds/dsPARAM_LIST.h"
#include "../ws/ds/dsEVENT.h"
#include "../ws/ds/dsEVENT_HANDLER.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsPAIR.h"
#include "../ws/ds/REF_TYPE.h"
#include "../ws/ds/dsREF_VECTOR.h"
#include "../ws/ssl/sslOBJ_REF.h"
#include "haloPLAYER_ACTION.h"
#include "haloRAW_INPUT_X360.h"

// Flattened boundary spelling used by the hcex bridge .cpp files (identical to the plain-C view's
// dsTSTRING_flat from hcex_ds_boundary.h). Layout-neutral alias of the canonical string template.
typedef dsTSTRING<char> dsTSTRING_flat;

typedef struct haloENGINE_CONTROL {
    // Nested ds-event subscriber that forwards dsEVENT_MGR signals back to the owning
    // haloENGINE_CONTROL (currently just gsUserConfigChanged -> OnConfigChanged). DB-verified
    // layout (types_members haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL): base
    // dsEVENT_HANDLER @0 (vftable, 4 bytes), pObj@4 (haloENGINE_CONTROL*) — size 8.
    struct EVENT_DISP_haloENGINE_CONTROL : public dsEVENT_HANDLER {
        haloENGINE_CONTROL *pObj; // 0x04 owning control object (null until SetProduct)

        // 0x823C22CC — construct with no owner and the base vtable installed.
        EVENT_DISP_haloENGINE_CONTROL();
        // 0x823C97AC — unsubscribe from every event in GetEventMap(), then restore the
        // dsEVENT_HANDLER base vtable pointer.
        ~EVENT_DISP_haloENGINE_CONTROL(); // 'override' dropped: dsEVENT_HANDLER base is an opaque boundary type

        // 0x823C989C — look up `event->id` in GetEventMap() (binary search) and invoke the bound
        // haloENGINE_CONTROL member function on pObj with (event->userId, event->paramList). A
        // no-op while pObj is null (before SetProduct).
        void OnEvent(const dsEVENT *event); // overrides dsEVENT_HANDLER slot 1 (virtual)

        // Compiler-generated (`??_G` mangle) scalar deleting destructor: runs ~EVENT_DISP_
        // haloENGINE_CONTROL(), then conditionally operator-deletes `this` when bit 0 of
        // `deleteFlags` is set.
        EVENT_DISP_haloENGINE_CONTROL *scalarDeletingDtor(unsigned char deleteFlags);

        // 0x823C4B78 — the shared (id -> handler) sorted table for every event this class
        // subscribes to. Function-local-static Meyers singleton, lazily InsertSorted-populated by
        // InitEventMap/RegisterEventEntry.
        static dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8> *GetEventMap();

        // 0x823CC488 — construct one (id, handler) pair and InsertSorted it into GetEventMap()
        // (INS_DUP_IGNORE: a duplicate id keeps the first registration).
        static void RegisterEventEntry(int id, void (haloENGINE_CONTROL::*handler)(unsigned long, const dsPARAM_LIST &));

        // 0x823CC4E8 — one-time (function-local-static guarded) registration of every event this
        // class handles: currently just "gsUserConfigChanged" -> OnConfigChanged.
        static void InitEventMap();

        // 0x823CC5D8 — bind this dispatcher to its owning object: sets pObj, then (only when
        // non-null) runs InitEventMap() once and SubscribeEvent()s every entry in GetEventMap()
        // against the global dsEVENT_MGR.
        void SetProduct(haloENGINE_CONTROL *_pObj);
    };

    EVENT_DISP_haloENGINE_CONTROL eventDispatcher;   // 0x00 forwards gsEventMgr signals to this object
    sslOBJ_REF                    sslObject;         // 0x08 the "haloEngine" SSL script object
    dsTSTRING<char>                curCheckpoint;     // 0x0C name of the most recently loaded/saved checkpoint
    dsTSTRING<char>                curLevel;          // 0x10 name of the currently loaded level/map
    dsTSTRING<char>                pathToCheckpoints; // 0x14 root directory for checkpoint files
    bool                           isCoop;            // 0x18 running in co-op mode

    // 0x823CAF50 — construct: install the event-dispatcher vtable (no owner yet), default-construct
    // the SSL object ref, initialise all three dsTSTRING<char> members to the shared empty string,
    // and clear the co-op flag.
    haloENGINE_CONTROL();
    // 0x823CAC60 — release the three dsTSTRING<char> buffers (refcount-decrement under the ds_string
    // processor lock), destroy the SSL object ref, then the event dispatcher.
    ~haloENGINE_CONTROL();

    // 0x823CCB00 — one-time bring-up: register the "haloEngine" SSL class/object (via
    // SSL_EXPORTER_haloENGINE_CONTROL), wire the input system's per-frame update callback, and read
    // pathToCheckpoints from config (defaulting to "d:\checkpoints", trailing-slash-normalized).
    // Returns false if hcex_get_local_player_profile-style setup never runs (see .cpp for the exact
    // failure path reproduced from the decompiler).
    bool Init();

    // 0x823CC6B0 — apply both primary and secondary users' persisted control settings, then bind
    // the event dispatcher to this object.
    void InitSettings();

    // 0x823B2B6C — name of the currently loaded level/map.
    const dsTSTRING<char> &GetCurLevel() const;
    // 0x823C1344 — name of the most recently loaded/saved checkpoint.
    const dsTSTRING<char> &GetCurCheckpoint() const;
    // 0x823C134C — root directory for checkpoint files.
    const dsTSTRING<char> &GetPathToCheckpoints() const;
    // 0x823C1330 — switch co-op/solo mode.
    void SetMode(bool _isCoop);
    // 0x823C133C — whether the engine is currently running in co-op mode.
    bool IsCoop() const;
    // 0x823C4B54 — set the current checkpoint name.
    void SetCurCheckpoint(const dsTSTRING<char> &chpt);

    // 0x823C1468 — whether a deterministic co-op action is currently pending for `input`
    // (hcex_coop_get_player_action's result reinterpreted as a bool).
    bool GetDetAction(haloPLAYER_ACTION *input);
    // 0x823C1490 — hand a digested deterministic action off to the Blam co-op update path.
    void SetDetAction(haloPLAYER_ACTION *actions);
    // 0x823C14C4 — record the second (remote) player's raw input for this tick.
    void SetRawInput(const haloRAW_INPUT_X360 &inp0, const haloRAW_INPUT_X360 &inp1);
    // 0x823C23C0 — fetch the raw input last recorded for `userId`.
    void GetRawInput(int userId, haloRAW_INPUT_X360 *inp);

    // 0x823C14E0 — per-frame Blam-side tick hook; currently a no-op stub.
    void ProcessNextFrame();

    // 0x823C14FC — abandon the current save state and reload the current map from scratch.
    void RestartLevel();
    // 0x823C630C — reload the current checkpoint (validating it first); invalidates the saved
    // game and reverts the map when the checkpoint file is missing/unreadable.
    void RestartCheckpoint();

    // 0x823C7008 — load `map` in solo mode at `difficulty`, optionally starting a new save
    // (deferring the map change until the next frame when this is the very first solo load).
    void LoadLevelSolo(dsTSTRING<char> map, short difficulty, bool isNew);
    // 0x823C9DAC — load `map` in co-op mode at `difficulty`, optionally starting a new save and/or
    // joining an existing online session (wiring up the relay/local-client-id and the deathless-
    // player debug cheat for the online case).
    void LoadLevelCoop(dsTSTRING<char> map, short difficulty, bool isNew, bool isOnLine);
    // 0x823C72F4 — return to the main menu (ui.map), clearing the deferred-first-solo-load flag.
    void LoadMainMenu();
    // 0x823C78D0 — load a named checkpoint file (queued for the next map revert if none is
    // currently loaded).
    void LoadCheckpoint(const dsTSTRING<char> &checkpoint);
    // 0x823CA034 — DEVIATION: the DB's raw prototype mistypes the hidden sret result pointer as a
    // second `haloENGINE_CONTROL*` parameter (classic PPC struct-return-by-value ABI artifact — see
    // CLAUDE.md's decompiler gotchas and the sibling haloENGINE_CONTROL_ssl_bridge.h note). Real
    // signature returns ds_data::REF_TYPE<dsREF_VECTOR> by value (sret): every checkpoint file found
    // under pathToCheckpoints/level[/coop] whose extension matches hcex_chpt_ext, as leaf names.
    ds_data::REF_TYPE<dsREF_VECTOR> CreateCheckpointList(const dsTSTRING<char> &level);

    // 0x823CB1C8 — pause/unpause the simulation (also silences both gamepads' vibration on pause).
    void SetPause(bool isOn);
    // 0x823CC828 — activate/deactivate the input system's general (non-debug) binding context;
    // also toggles the HCEX input-suppression flag and clears digested X360 input while in Halo mode.
    void SwitchInput(bool isOn);

private:
    // 0x823C7F40 — apply one user slot's (primary or secondary) persisted controller settings from
    // its per-user apCONFIG onto its live player_profile (invert-look/flight/autocenter,
    // look-sensitivity, joystick-vibration-off, and the named button/stick layout presets).
    void ApplySettings(bool isPrimary);
    // 0x823C89C8 — gsUserConfigChanged event handler: re-applies settings for whichever user slot
    // (primary/secondary) the signaled userID matches.
    // DEVIATION: `unsigned long`, not `unsigned int`. The binary's mangled symbol
    // ?OnConfigChanged@haloENGINE_CONTROL@@AAAXKABVdsPARAM_LIST@@@Z spells the first parameter K
    // = unsigned long, which is also RegisterEventEntry's member-fn-ptr parameter above; the two
    // are distinct MSVC types at the same width, and the mismatch needed a cast to bridge.
    void OnConfigChanged(unsigned long eventUserId, const dsPARAM_LIST &paramList);
} haloENGINE_CONTROL;
