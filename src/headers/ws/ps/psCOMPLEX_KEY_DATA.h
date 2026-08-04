#pragma once
#include "../ds/dsSTATIC_VECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine ps: the parsed form of a (possibly dotted / bracketed) "complex" config key. The key
// text is split into a chain of interned sub-key ids (`_psKeysArr`, e.g. "a.b.c" -> {a,b,c}) plus
// a scratch string buffer (`_psSubStr`) used while splitting. Built by
// psSECTION_INTERFACE::SplitComplexKey and consumed by the AddKey / GetValue paths.
// DB-verified layout (types_members psCOMPLEX_KEY_DATA):
//   _psKeysArr@0 (dsSTATIC_VECTOR<dsSTRID,20>, 84 bytes), _psSubStr@84 (dsTSTRING<char>) — size 88.

typedef struct psCOMPLEX_KEY_DATA {
    dsSTATIC_VECTOR<dsSTRID, 20> _psKeysArr; // 0x00 chain of interned sub-key ids
    dsTSTRING<char>              _psSubStr;  // 0x54 scratch split buffer (ref-counted)

    psCOMPLEX_KEY_DATA(); // boundary — default construct (empty key chain, shared-empty _psSubStr)
} psCOMPLEX_KEY_DATA;
