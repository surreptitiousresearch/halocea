#pragma once
// ws-engine vid: one GPU-buffer backing store record. DB-verified layout (types_members
// vidBUFFER, size 16). `curOfs` is the dynamic write cursor rewound by DiscardDynAll; `allocated`
// is the byte size committed; the platform hardware-buffer header is stored 0x68 bytes ahead of
// this record (see d3dVBUF_MNG::GetDynHwB).

#include "../ap/apSTATE_T.h"

typedef struct vidBUFFER
{
    apSTATE_T<unsigned char> flags;       /* 0x00 */
    unsigned char            supportData; /* 0x01 */
    unsigned char            _pad02[2];   /* 0x02 */
    int                      curOfs;      /* 0x04 dynamic write cursor */
    int                      allocated;   /* 0x08 committed byte size */
    int                      idx;         /* 0x0C */
} vidBUFFER; /* 16 bytes */
