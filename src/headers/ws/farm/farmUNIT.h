#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../ds/WEAK_PTR.h"
#include "farmCREATE_DATA.h"
#include "farmCOND_DATA.h"
// ws-engine farm: one farmable entity slot (creation record + condition data + the currently
// materialized entity, if any). DB-verified layout (types_members farmUNIT) — size 308.

struct entENTITY;

struct farmUNIT {
    apSTATE_T<unsigned char> state;           // 0x000
    unsigned char            _pad01[3];       // 0x001 db-verified padding
    farmCREATE_DATA          createData;      // 0x004 (272B)
    farmCOND_DATA            condData;        // 0x114 (16B)
    ds::WEAK_PTR<entENTITY>  entCurrent;      // 0x124 materialized entity (weak)
    int                      visCheckedFrame; // 0x128
    uint16_t                 visDomCache[2];  // 0x12C
    int                      uid;             // 0x130
}; // 308 bytes
