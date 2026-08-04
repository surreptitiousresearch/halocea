#pragma once
#include "sslCLASS_REF.h"
// ssl foreach-loop cursor record (element of dsVECTOR<sslFOREACH_DATA,8>).
// DB-verified layout (types_members sslFOREACH_DATA): checkClass@0, lastObjIdx@4 — size 8.
// sslCLASS_REF (a single ref-counted pointer to a script class descriptor) is defined in
// sslCLASS_REF.h.

typedef struct sslFOREACH_DATA {
    sslCLASS_REF checkClass; // 0x00 class filter for the iteration
    int          lastObjIdx; // 0x04 index of the last object visited
} sslFOREACH_DATA;

// 0x82AF0420 — placement-copy-construct helper: `template<> sslFOREACH_DATA *New<sslFOREACH_DATA>
// (sslFOREACH_DATA *place, const sslFOREACH_DATA *val)` in sslFOREACH_DATA__New.cpp (specializes the
// generic New<T> placement-copy helper used across the codebase; not a member function).
