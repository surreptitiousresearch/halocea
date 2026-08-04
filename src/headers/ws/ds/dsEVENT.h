#pragma once
#include "dsPARAM_LIST.h"
// ws-engine ds: the event instance handed to a dsEVENT_MGR subscriber's OnEvent/callback (distinct
// from dsEVENT_MGR::EVENT, the larger internal pool record it is projected from at dispatch time).
// DB-verified layout (types_members dsEVENT): id@0 (int), userId@4 (unsigned int),
// paramList@8 (dsPARAM_LIST) — size 16.

typedef struct dsEVENT {
    int          id;        // 0x00 registered event handle (dsEVENT_MGR::RegisterEvent's return)
    unsigned int userId;    // 0x04 originating/target thread-local user id
    dsPARAM_LIST paramList; // 0x08 named parameters attached to this signal
} dsEVENT;
