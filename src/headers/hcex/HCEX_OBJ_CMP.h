#pragma once
#include "HCEX_OBJ.h"
// HCEX bridge: stateless ascending-id comparator for the hcexObjects sorted vector
// (dsVECTOR<HCEX_OBJ,8>::FindSorted / InsertSorted). DB-verified size (types_members
// HCEX_OBJ_CMP) — 1 (empty class).

typedef struct HCEX_OBJ_CMP {
    // 0x823ED73C — three-way compare by HCEX_OBJ::id (const). The decompiler renders this as an
    // XOR-based overflow-safe compare; verified equivalent (by truth table over the sign/overflow
    // cases) to a plain signed 3-way compare on `id`. DEVIATION: simplified to the plain form.
    int cmp(const HCEX_OBJ &a, const HCEX_OBJ &b) const;

    // 0x823ED708 — overload comparing against a raw id (used by FindSorted's key search).
    // Same DEVIATION as above.
    int cmp(const HCEX_OBJ &a, int b) const;
} HCEX_OBJ_CMP;
