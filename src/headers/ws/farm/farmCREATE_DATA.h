#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
#include "../../entCREATE_DATA.h"
// ws-engine farm: deferred entity-creation record kept per farm unit. DB-verified layout
// (types_members farmCREATE_DATA) — size 272.

struct farmCREATE_DATA {
    dsTSTRING<char> nameTpl;     // 0x000 name-template string
    dsTSTRING<char> nameClass;   // 0x004 entity class name
    unsigned int    mpID;        // 0x008
    void           *savedData;   // 0x00C
    int             sdSize;      // 0x010 savedData size
    bool            isLightData; // 0x014
    unsigned char   _pad15[3];   // 0x015 db-verified padding
    entCREATE_DATA  cd;          // 0x018 (236B)
    m3dV            posVis;      // 0x104 visibility-check position
}; // 272 bytes
