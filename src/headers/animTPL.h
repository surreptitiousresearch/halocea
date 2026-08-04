#pragma once
/* animTPL — a ws-engine animated template (the shared mesh/collision/animation asset backing an
 * animINST). Boundary type; full 280-byte (0x118) layout DB-verified against types_members animTPL.
 * Pointer members carry forward-declared typed pointees. */

#include "ws/anim/animTPL_COLL.h"
#include "ws/ld/ldMOPPCode.h"
#include "ws/ds/dsVECTOR.h"
#include "ws/ds/dsTSTRING.h"
#include "ws/ds/dsAFFIX_STRING.h"
#include "ws/m3d/m3dBOX.h"
#include "ws/ps/psSECTION.h"
#include "ws/anim/tplLOD_DEF.h"

typedef struct osLOCK               osLOCK;
typedef struct objOBJ               objOBJ;
typedef struct selSEL               selSEL;
typedef struct animBANK_SET         animBANK_SET;
typedef struct animTRACK            animTRACK;
typedef struct animTPL_DATA_HOLDER  animTPL_DATA_HOLDER;
typedef struct tplSKIN              tplSKIN;

typedef struct animTPL
{
    osLOCK       *cdtLocalLock;      /* 0x00 */
    animTPL_COLL  coll;              /* 0x04 collision block (mopps records) */
    dsVECTOR<ldMOPPCode, 8> moppsLd; /* 0x18 loaded MOPP code blobs */
    char          name[64];          /* 0x2C template name */
    unsigned int  sign;              /* 0x6C */
    unsigned int  id;                /* 0x70 */
    char          nameClass[64];     /* 0x74 */
    int           state;             /* 0xB4 */
    unsigned __int8 _reserved0 : 6;  /* 0xB8 padding bits .0-.5 (unnamed in DB) */
    unsigned __int8 isSpecMesh : 1;  /* 0xB8 bit .6 */
    unsigned __int8 isMtlListInited : 1; /* 0xB8 bit .7 */
    unsigned char _padB9[3];         /* 0xB9 */
    objOBJ       *pObj;              /* 0xBC */
    m3dBOX        bbox;              /* 0xC0 default axis-aligned bounding box */
    int           nObjSel;           /* 0xD8 */
    selSEL       *objSelList;        /* 0xDC */
    animBANK_SET *animBankSet;       /* 0xE0 */
    psSECTION     ps;                /* 0xE4 default section this template's instances copy */
    dsTSTRING<char> *navDataPS;      /* 0xE8 */
    dsVECTOR<unsigned char, 8> *navDataBin; /* 0xEC */
    animTRACK    *trackAnim;         /* 0xF0 */
    dsTSTRING<char> *optimDataPS;    /* 0xF4 */
    animTPL_DATA_HOLDER *tplExtData; /* 0xF8 */
    dsVECTOR<tplLOD_DEF, 8> lodDef;  /* 0xFC level-of-detail definition records */
    tplSKIN      *pSkin;             /* 0x110 */
    dsAFFIX_STRING affixes;          /* 0x114 */
} animTPL; /* 280 bytes */
