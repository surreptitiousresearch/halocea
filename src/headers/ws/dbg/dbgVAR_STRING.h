#pragma once
#include "../wb/dbgVAR_boundary.h"
// ws-engine dbg: a string-typed debug variable -- a named leaf of the dbgVAR hierarchy shared with
// wb (watch/debug window). Sibling leaves seen in the same DB grep: dbgVAR_BOOL, dbgVAR_INT,
// dbgVAR_FLOAT, dbgVAR_M3DV, dbgVAR_M3DCOLOR, dbgVAR_TRIGGER, dbgVAR_STRING_LIST.
// DB-verified layout (types_members dbgVAR_STRING): a single anonymous base-class member of type
// dbgVAR_SIMPLE<dsTSTRING<char>,4> at offset 0, size 24 -- modeled as public inheritance (promotes
// dbgVAR/dbgVAR_IMPL's fields, matches layout). Own vtable identity (dbgVAR_STRING_vtbl) overrides
// the same 4 slots as the base (dtr/GetAsPsData/ReadPs/Update); reuses the dbgVAR_vtbl shape per
// the existing dbgVAR_SIMPLE<T,N> convention.
//
// Canonical home, and as of 2026-08-07 the SOLE definition: hcex/hcex_dbgvar_boundary.h carried a
// second body deriving from dbgVAR_IMPL (one level too low against the DB row above) and now
// includes this header instead (odr_dup drain).

struct dbgVAR_STRING : dbgVAR_SIMPLE<dsTSTRING<char>, 4> {
    static dbgVAR_vtbl vftable; // `dbgVAR_STRING::`vftable''  -- boundary, not decompiled

    // ?CStr@dbgVAR_STRING@@QBAPBDXZ @ 0x823C2358 -- format/return the string variable's current
    // value as a NUL-terminated C string (borrows the shared dsTSTRING buffer). Const (QBA).
    const char *CStr() const;
};
