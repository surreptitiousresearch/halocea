#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ps: a legacy ("old") string-valued property node, kept for backward-compatible config
// text. DB-verified layout (types_members psOLD): str@0 (dsTSTRING<char>) — size 4.

typedef struct psOLD {
    dsTSTRING<char> str; // 0x00 raw stored text
} psOLD;
