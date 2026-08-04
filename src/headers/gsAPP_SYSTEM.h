#pragma once

/* ws-engine application-system root object (gsAPP_SYSTEM). DB-verified layout
   (types_members gsAPP_SYSTEM, size 352). Members typed against their reversed
   home headers (dsTSTRING, dsVECTOR, fioFILE_DISK); no layout is invented. */

#include "ws/ds/dsTSTRING.h"
#include "ws/ds/dsVECTOR.h"
#include "ws/fio/fioFILE_DISK.h"

typedef struct gsAPP_SYSTEM_vtbl gsAPP_SYSTEM_vtbl;

typedef struct gsAPP_SYSTEM
{
    gsAPP_SYSTEM_vtbl            *__vftable;        /* 0x00 */
    dsTSTRING<char>              cmdLine;           /* 0x04 */
    dsVECTOR<dsTSTRING<char>,8>  anitecNames;       /* 0x08 */
    int                          anitecShotIdx;     /* 0x1C */
    bool                         anitecSoundOnly;   /* 0x20 */
    unsigned char _pad0[3]; /* db-verified padding */
    /* 0x21 padding[3] */
    fioFILE_DISK                 anitecLog;         /* 0x24 */
} gsAPP_SYSTEM; /* 352 bytes */
