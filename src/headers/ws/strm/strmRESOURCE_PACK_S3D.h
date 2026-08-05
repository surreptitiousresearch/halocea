#pragma once
#include "strmRESOURCE.h"
#include "strmPACK_S3D.h"
#include "../ds/dsSTRID.h"
// ws-engine strm (streaming): CRTP mixin that adds "can this resource be read out of a
// strmPACK_S3D pack instead of its loose file" behavior on top of a strmRESOURCE-derived base.
// Only the <strmRESOURCE, strmRESOURCE_TEXTURE> instantiation is reversed/DB-verified in this
// batch (types_members strmRESOURCE_PACK_S3D<strmRESOURCE,strmRESOURCE_TEXTURE>): base BASE@0,
// idRec@8, pack@12 (strmPACK_S3D*), resourceName@16 (dsSTRID), pakId@20 — size 24. Other
// instantiations (<strmRESOURCE_WAITABLE,gmEXT_TPL_STRM_RES>, <strmRESOURCE_WAITABLE,
// strmRESOURCE_TPL>) exist in the binary but are out of scope here.

template<class BASE, class DERIVED>
struct strmRESOURCE_PACK_S3D : BASE {
    int          idRec;        // 0x08 record index within `pack`'s recTable, once resolved
    strmPACK_S3D *pack;         // 0x0C the pack this resource is read from, if IsReadFromPack()
    dsSTRID       resourceName; // 0x10
    int           pakId;        // 0x14 sub-pak/language variant selector

    ~strmRESOURCE_PACK_S3D();
    int GetAdditionalSeek(); // boundary — external to this batch

    // 0x8266F6E0 <strmRESOURCE,strmRESOURCE_TEXTURE> — reads the "Preload.usePaks" config key
    // (cached in a function-local static after the first successful read) and returns whether it
    // is set to exactly 1.
    bool IsReadFromPack();

    // 0x82670D20 <strmRESOURCE,strmRESOURCE_TEXTURE> — if pack-reading is enabled, resolve this
    // resource's pack/record via strmPACK_MANAGER::CanOpenResourceFile + strmPACK_S3D::FindPakRec
    // (leaving `pack` null and idRec 0 on a miss); either way, hand off to
    // strmSTREAMING_MANAGER::AddQuery to actually enqueue the load.
    // Mangled return type is bool (_N) — DEVIATION: the decompiler shows this returning
    // `strmPACK_S3D *`, a spurious reinterpretation of strmSTREAMING_MANAGER::AddQuery's bool
    // result; the mangled name (`?AddQuery@...@@QAA_NXZ`) is authoritative.
    bool AddQuery();
};
