#pragma once
#include "../ps/psSECTION.h"
#include "../ds/MAP.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
// ws-engine ap (platform): the config/exports database — a psSECTION-backed key/value store plus
// an "exportMap" that lets a config key resolve directly to a dsSTATE_MGR-tracked state id
// (bypassing the section parse for values registered via the ap export path).
// DB-verified layout (types_members apCONFIG): __vftable@0, ps@4 (psSECTION), exportMap@8
// (ds::MAP<dsSTRID,int,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>, 84B), state@92 (unsigned int),
// diffID@96 (dsDATA, 8B), baseID@104 (dsDATA, 8B), version@112 (int) — size 116.

struct apCONFIG_vtbl; // boundary — ap subsystem vtable
struct dsSTATE_MGR;   // ../ds/dsSTATE_MGR.h — boundary (pointer only)
extern dsSTATE_MGR *gStateMgr;

typedef struct apCONFIG {
    apCONFIG_vtbl                                               *__vftable;  // 0x00
    psSECTION                                                     ps;        // 0x04
    ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> exportMap; // 0x08 config-key -> dsSTATE_MGR state id
    unsigned int                                                   state;     // 0x5C
    dsDATA                                                         diffID;    // 0x60
    dsDATA                                                         baseID;    // 0x68
    int                                                             version;   // 0x70

    // 0x82623188 — resolve `complexKey`: if it's registered in `exportMap` (and gStateMgr is
    // live), return a copy of the tracked dsSTATE_MGR state directly; otherwise fall back to a
    // plain psSECTION lookup on `ps`. const (QBA).
    // DEVIATION: the raw decompile's `this`/`result` register labeling is inconsistent past the
    // exportMap lookup (the sret dsDATA pointer and the `this` apCONFIG pointer both get
    // register r3 at different points and the decompiler keeps calling both "this"); reconstructed
    // here as ordinary sret-returning C++ using the two objects the disassembly actually touches
    // (exportMap on `this`, the output dsDATA on the hidden return slot).
    dsDATA GetRawValue(const char *complexKey) const;

    // Store a typed value under `complexKey`. `setCurrent` selects the current-value store (1) vs
    // the default/base store (0). Returns false when the key has no matching default entry.
    // boundary — per-type instantiation reversed elsewhere.
    template<class T>
    bool SetValue(const char *complexKey, const T *value, int setCurrent);

    // Retrieve a typed value for `complexKey`: fetch the raw dsDATA (GetRawValue) then convert.
    // Tries the fast retrieve conversion first, then an explicit conversion if the key exists;
    // logs and returns false on miss/failure. 0x823C4290 is the <bool> instantiation, reversed in
    // apCONFIG__GetValue_bool.cpp. Other instantiations (e.g. <int>) are boundary here.
    template<class T>
    bool GetValue(const char *complexKey, T *val) const;

    // 0x823D5E48 — Retrieve<int,int>: build "<section>.<key>", look it up via GetValue<int>, and
    // fall back to `defVal` when absent. Reversed in apCONFIG__Retrieve_int.cpp.
    // DEVIATION: the decompiler packed the two `const char*` args into one __int64 and mislabeled
    // the trailing refs; the disassembly (r4=section, r5=key, r6=val, r7=defVal) confirms the
    // 4-argument shape below.
    template<class TVal, class TDef>
    void Retrieve(const char *section, const char *key, TVal &val, const TDef &defVal);
} apCONFIG;
