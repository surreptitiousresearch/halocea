#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../m3d/m3dV.h"
// ws-engine rend: the frame's camera list (huge — 47136 bytes, dominated by 50 embedded
// rendCAM records). DB-verified layout (types_members rendCAM_LIST): state@0, list@4
// (dsCONST_ARRAY<rendCAM,50>, 47004B), origins@47008 (dsCONST_ARRAY<m3dV,10>, 124B),
// nColorCam@47132 — size 47136.

#include "rendCAM.h" // full type required: dsCONST_ARRAY<rendCAM,50> embeds rendCAM by value

typedef struct rendCAM_LIST {
    apSTATE_T<unsigned long>  state;    /* 0x0000 */
    dsCONST_ARRAY<rendCAM,50> list;     /* 0x0004 */
    dsCONST_ARRAY<m3dV,10>    origins;  /* 0xB7A0 */
    int                       nColorCam;/* 0xB81C */
} rendCAM_LIST; /* 47136 bytes */
