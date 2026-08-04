#pragma once
#include "psSECTION.h"
// ws-engine ps: a located key reference — a section handle plus the record index of a key within
// that section's ordered-record table. Returned by psSECTION_DATA::GetValueRefs so the caller can
// read the record's name / line / value.
// DB-verified layout (types_members psSECTION_KEY_REF): ps@0 (psSECTION), keyIdx@4 (int) — size 8.

typedef struct psSECTION_KEY_REF {
    psSECTION ps;     // 0x00 section body holding the key
    int       keyIdx; // 0x04 record index of the key within that section
} psSECTION_KEY_REF;
