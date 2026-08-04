#pragma once
#include "../ds/dsSTRID.h"
#include "sslCLASS_REF.h"
// ssl subsystem: one entry of sslSYSTEM::namedClasses (dsVECTOR<sslCLASS_MAP_ELEM,8>) — an
// interned class-name -> class-reference map, kept sorted by name-id for binary search.
// DB-verified layout (types_members sslCLASS_MAP_ELEM): name@0 (dsSTRID), cls@4 (sslCLASS_REF) — size 8.

typedef struct sslCLASS_MAP_ELEM {
    dsSTRID      name; // 0x00 interned class name
    sslCLASS_REF cls;  // 0x04 reference to the class descriptor
} sslCLASS_MAP_ELEM;
