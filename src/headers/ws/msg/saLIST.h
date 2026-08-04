#pragma once
#include "../ds/LIST.h"
// ws-engine msg: one (message-id -> subscriber addresses) entry in a msgSUBSCRIBER's table.
// DB-verified layout (types_members saLIST): msg@0, addList@4 (ds::LIST<msgADDR *>) — size 24.

struct msgADDR;

typedef struct saLIST {
    int                 msg;     // 0x00 message id this subscriber list is keyed on
    ds::LIST<msgADDR *> addList; // 0x04 addresses registered to receive `msg`
} saLIST;
