#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
// ssl subsystem: a script function/event signature. DB-verified layout
// (types_members sslFUNCTION_DECL): name@0, pRetType@4, param@8 — size 28.

struct dsDATA_TYPE; // boundary — runtime type descriptor (full definition in ds/dsDATA_TYPE.h)

typedef struct sslFUNCTION_DECL {
    // One formal parameter. DB-verified (types_members sslFUNCTION_DECL::PARAM):
    //   pType@0, name@4, defValue@8 (dsDATA) — size 16.
    struct PARAM {
        const dsDATA_TYPE *pType;    // 0x00 declared parameter type
        dsSTRID            name;     // 0x04 parameter name
        dsDATA             defValue; // 0x08 default value (empty when none)
    };

    dsSTRID            name;     // 0x00 function name
    const dsDATA_TYPE *pRetType; // 0x04 return type (null = void)
    dsVECTOR<PARAM, 8> param;    // 0x08 formal parameters
} sslFUNCTION_DECL;
