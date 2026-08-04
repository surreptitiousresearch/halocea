#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine mtl: material manager. Builds/filters "affix strings" — texture/material
// name suffixes derived from an objOBJ's state (used to pick material variants).
// DB-verified layout (types_members mtlMTL_MNG): inited@0 (bool) — size 1.

struct objOBJ; // ../obj/objOBJ.h — pointer only

typedef struct mtlMTL_MNG {
    bool inited; // 0x00

    // 0x82A86B30 — concatenate `instAffixes` with `pObj`'s own dsAFFIX_STRING (objOBJ::affixes
    // at offset 0x34) into a single affix string, returned by value (sret). `this` is loaded into
    // r4 by the ABI but never read by the body — the method does not touch manager state.
    dsTSTRING<char> BuildMtlAffixString(objOBJ *pObj, const dsTSTRING<char> &instAffixes);

    // 0x82A86A50 — hash an affix string down to an 8-hex-digit CRC32 suffix ("%08X"); an empty
    // input adopts the shared empty-string singleton instead of hashing. Protected (IAA mangle),
    // const (QBA mangle promoted from disasm: r4/this unused by body). `this` likewise unused.
    dsTSTRING<char> _MtlSuffixFromAffixString(const dsTSTRING<char> &affixStr) const;

    // 0x82A86ED0 — parse `affixString` into a dsAFFIX_BUILDER, keep only the affixes whose name
    // contains the substring "vid.sdr.", and rebuild the filtered set back into a single string
    // (sret). const; `this` is loaded per the ABI but never read by the body.
    dsTSTRING<char> FilterAffixStringOverride(const dsTSTRING<char> &affixString) const;
} mtlMTL_MNG;
