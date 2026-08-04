#pragma once
#include "ws/ds/dsCONST_ARRAY.h"

// d3d render-pass descriptors. DB-verified layouts (types_members d3dOBJ_PASS /
// d3dOBJ_PASS_LIST).

// One compiled pass reference — size 10.
typedef struct d3dOBJ_PASS
{
    unsigned __int16 sdrDesc;   /* 0x00 */
    unsigned __int16 rsBlock;   /* 0x02 */
    unsigned __int16 fvfNmb;    /* 0x04 */
    unsigned __int8  id;        /* 0x06 */
    unsigned __int8  rendFlags; /* 0x07 */
    unsigned __int16 sdrPair;   /* 0x08 */
} d3dOBJ_PASS;

// Pass set for one draw-batch split — size 88.
typedef struct d3dOBJ_PASS_LIST
{
    dsCONST_ARRAY<d3dOBJ_PASS, 2> passColor[2]; /* 0x00 */
    d3dOBJ_PASS                   passZOnly;    /* 0x30 */
    unsigned char                 _pad3A[2];    /* 0x3A db-verified padding */
    dsCONST_ARRAY<d3dOBJ_PASS, 1> passDUDV;     /* 0x3C */
    d3dOBJ_PASS                   passSM;       /* 0x4C */
    unsigned char                 _pad56[2];    /* 0x56 db-verified tail padding */
} d3dOBJ_PASS_LIST;
