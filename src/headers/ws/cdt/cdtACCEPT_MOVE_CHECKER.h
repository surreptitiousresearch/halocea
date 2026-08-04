#pragma once
// ws-engine cdt: cached "may I move there" collision probe — wraps a sphere-cast query and
// memoizes the last answer/time. DB-verified layout (types_members cdtACCEPT_MOVE_CHECKER):
// __vftable@0, lastAcceptMoveTime@4, srQueryAcceptMove@8, lastRes@12 — size 16.
// Explicit __vftable member + no virtual methods (cdtREFINE precedent — keeps sizeof exact
// without a compiler-generated vptr).

struct cdtACCEPT_MOVE_CHECKER_vtbl; // boundary
struct srQUERY_CSPHERE;             // sr — sphere-cast query   boundary (ptr)

struct cdtACCEPT_MOVE_CHECKER {
    cdtACCEPT_MOVE_CHECKER_vtbl *__vftable;          // 0x00
    float                        lastAcceptMoveTime; // 0x04
    srQUERY_CSPHERE             *srQueryAcceptMove;  // 0x08
    bool                         lastRes;            // 0x0C
    unsigned char                _pad0D[3];          // 0x0D
};                                                   // 16 bytes
