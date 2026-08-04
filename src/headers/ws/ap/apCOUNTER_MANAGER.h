#pragma once
#include "../con_/CONSOLE_CALLBACK.h"
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "apSTATE_T.h"
#include "apCOUNTER_TRACKER.h"
// ws-engine ap (platform): global registry of runtime performance counters. A console page
// (derives con::CONSOLE_CALLBACK) that owns the counter list and forwards them to the console.
// DB-verified layout (types_members apCOUNTER_MANAGER) — size 180.

struct apCOUNTER;                    // boundary — ap subsystem
struct apPROFILER;                   // boundary — ap subsystem
struct camCAMERA_SETTER_ABSTRACT;    // boundary — cam subsystem

struct apCOUNTER_MANAGER_vtbl; // boundary — ap subsystem vtable

struct apCOUNTER_MANAGER : con::CONSOLE_CALLBACK {
    static apCOUNTER_MANAGER_vtbl vftable; // `apCOUNTER_MANAGER::`vftable'' -- boundary, not decompiled

    dsVECTOR<apCOUNTER *, 8>  counters;       // 0x10 all registered counters
    bool                      isChanged;      // 0x24
    unsigned char             _pad25[3];      // 0x25
    dsVECTOR<apCOUNTER *, 8>  countersToSend; // 0x28 pending console pushes
    bool                      isFirstUpdate;  // 0x3C
    apSTATE_T<unsigned char>  commonSettings; // 0x3D
    unsigned char             _pad3E[2];      // 0x3E
    dsVECTOR<apPROFILER *, 8> profilers;      // 0x40
    apCOUNTER_TRACKER         cntTracker;     // 0x54
    camCAMERA_SETTER_ABSTRACT *cameraManager; // 0x7C
    osLOCK                    lock;           // 0x80

private:
    // 0x82715E00 — private (AAA mangle: private, non-virtual) default ctor; only Instance()
    // constructs the singleton. Chains con::CONSOLE_CALLBACK, zero-inits the dsVECTORs/state,
    // default-constructs cntTracker, then inits `lock`.
    apCOUNTER_MANAGER();

public:
    // 0x82715EF0 — Meyers singleton accessor.
    static apCOUNTER_MANAGER &Instance();

    // 0x82718E60 — register `counter`: forward late-add handling to the tracker if running,
    // append to the counter list (and the pending-send list on the first update), and return
    // the counter's index. Thread-safe (holds `lock`).
    int AddCounter(apCOUNTER *counter);

    // 0x82716670 — true if `name` already matches a registered counter's name (byte-wise compare).
    // const (ABA mangle), returns bool (_N).
    bool IsDuplicatedCntName(const dsTSTRING<char> *name) const;
};
