#pragma once
#include "mdlCDT_SETTINGS.h"
#include "../ds/dsSTRID.h"
// ws-engine mdl: descriptor block shared by a body's model info. DB-verified layout
// (types_members mdlINFO_BASE_DESC): __vftable@0, cdtSettings@4, refine* strids@0x28..0x34.

struct mdlINFO_BASE_DESC {
    void            *__vftable;     // 0x00 mdlINFO_BASE_DESC_vtbl*
    mdlCDT_SETTINGS  cdtSettings;   // 0x04 collision-detection tuning (size 0x24)
    dsSTRID          refineFloor;   // 0x28
    dsSTRID          refineSphere;  // 0x2C
    dsSTRID          refineBodies;  // 0x30
    dsSTRID          refineColorize;// 0x34
};
