#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ap/apSTATE_T.h"
#include "../lgt/lgtLIGHT.h"
// ws-engine rend: ref-counted wrapper around a runtime lgtLIGHT, held by the light manager's
// static/dynamic light vectors. DB-verified layout (types_members rendLIGHT) — size 112 (0x70).
// Per the project's boundary conventions only the members the light-set update reads are typed
// (the intrusive ref-count, the state word and the lgtLIGHT back-pointer); the rest is padding
// at its exact DB offset/size.

struct animINST; // ../anim/animINST.h — pointer only  boundary

typedef struct rendLIGHT {
    dsREF_COUNT               ref;       // 0x00 (DB: unnamed embedded dsREF_COUNT)
    apSTATE_T<unsigned long>  state;     // 0x04
    lgtLIGHT                 *lightData; // 0x08 underlying light record
    unsigned char             _pad0C[100]; // 0x0C .. 0x70 remaining fields  boundary
} rendLIGHT;
