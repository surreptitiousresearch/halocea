#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
// ws-engine ps: one named parameter record in a particle-section's sorted parameter table.
// DB-verified layout (types_members psSECTION_RECORD):
//   keyStrId@0 (dsSTRID), value@4 (dsDATA), sortedIdx@12 (int), lineNmb@16 (__int16) — size 20.

typedef struct psSECTION_RECORD {
    dsSTRID keyStrId;  // 0x00 interned parameter name
    dsDATA  value;     // 0x04 stored parameter value
    int     sortedIdx; // 0x0C index (into the owning vector) of the sort-order predecessor
    short   lineNmb;   // 0x10 source line number the record was parsed from

    // Default-construct a record (fields set piecewise by the caller, per the reversed AddRec path).
    psSECTION_RECORD();                             // boundary
    // Copy-construct / destroy (the dsDATA `value` deep-copies / releases its held value).
    // Reversed elsewhere; declared here so dsVECTOR<psSECTION_RECORD,8>::PushBack type-checks.
    psSECTION_RECORD(const psSECTION_RECORD &that); // boundary
    ~psSECTION_RECORD();                            // boundary
} psSECTION_RECORD;

// ORS_CMP — the comparator functor passed to dsVECTOR<psSECTION_RECORD,8>::FindSorted. It holds a
// pointer to the record vector and orders records by their interned key id (dsSTRID pointer value)
// via the per-record sortedIdx permutation.
// DB-verified layout (types_members ORS_CMP): arr@0 (dsVECTOR<psSECTION_RECORD,8>*) — size 4.
typedef struct ORS_CMP {
    dsVECTOR<psSECTION_RECORD, 8> *arr; // 0x00
} ORS_CMP;

// ORS_SWAP — the swap functor passed alongside ORS_CMP to dsVECTOR<psSECTION_RECORD,8>::Sort. It
// exchanges only the `sortedIdx` permutation field between two records (not the whole record).
// Stateless: the DB carries no members for it (types_members ORS_SWAP is empty).
typedef struct ORS_SWAP {} ORS_SWAP;
