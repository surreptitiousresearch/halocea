#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */

/* ws-engine application-system boundary (gsAPP_SYSTEM). This is a large
   ws-engine class; only the members HCEX touches are modeled. Layout offsets
   are DB-confirmed (types_members). Not reversed here — boundary type. */

#include "apCL.h"

/* dsVECTOR<dsTSTRING<char>,8> — 20 bytes (DB) */
typedef struct gsAPP_SYSTEM_anitec_names
{
    void         *pData;      /* 0x00 dsTSTRING<char> * */
    int           nElem;      /* 0x04 */
    int           allocated;  /* 0x08 */
    apCL          __cl;       /* 0x0C — DB: types_members dsVECTOR<dsTSTRING<char>,8> __cl : apCL */
} gsAPP_SYSTEM_anitec_names;

/* gsAPP_SYSTEM split out to its own DB-verified header. */
#include "gsAPP_SYSTEM.h"

extern gsAPP_SYSTEM *gsAppSystem;
