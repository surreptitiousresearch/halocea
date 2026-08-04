#pragma once
// ws-engine ai08: type-safe controller/interface index handle. A thin wrapper over an int
// index into a per-weapon controller list, templated on the controlled type so different
// index spaces (propWPN_AI_USABLE vs STRIKE_GROUP) do not mix.
// DB-verified layout (types_members aiW_IDX<propWPN_AI_USABLE>): idx@0 — size 4.

template<class T>
struct aiW_IDX {
    int idx; // 0x00 index into the owning weapon-module's list (-1 == none)
};
