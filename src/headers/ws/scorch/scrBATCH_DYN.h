#pragma once
#include "scrBATCH_BASE.h"
#include "scrTEXTURES.h"
#include "../ds/dsSMART_PTR.h"
// ws-engine scorch: a dynamic scorch batch (owned by scrSCORCH_MANAGER::dynScorchBatches).
// DB-verified layouts (types_members scrBATCH_DYN / scrBATCH_DYN_DESC / scrBATCH_DESC_BASE).

struct txmTEXTURE;    // ../txm texture object   -- boundary (pointer only)
struct rendLIGHT_SET; // ../rend/rendLIGHT_SET.h -- boundary (smart-ptr referent)
struct dsREF_COUNT;   // ../ds/dsREF_COUNT.h     -- boundary
struct objOBJ;        // obj subsystem           -- boundary (pointer only)
struct m3dMATR;       // ../m3d matrix           -- boundary (pointer only)

// DB-verified (types_members scrBATCH_DESC_BASE) -- size 36.
typedef struct scrBATCH_DESC_BASE {
    scrTEXTURES     textures;  // 0x00 (20)
    txmTEXTURE     *pBaseTex;  // 0x14
    unsigned __int8 blendType; // 0x18
    int             isGlowing; // 0x1C
    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > lset; // 0x20
} scrBATCH_DESC_BASE;

// DB-verified (types_members scrBATCH_DYN_DESC) -- size 44. Offset-0 base is a base class in DB.
typedef struct scrBATCH_DYN_DESC : scrBATCH_DESC_BASE {
    objOBJ        *pObjFollow;  // 0x24
    const m3dMATR *matrLTPrev;  // 0x28
} scrBATCH_DYN_DESC;

// DB-verified (types_members scrBATCH_DYN) -- size 404. Offset-0 base is a base class in DB.
typedef struct scrBATCH_DYN : scrBATCH_BASE {
    scrBATCH_DYN_DESC desc;    // 0x164 (44)
    bool              visible; // 0x190
} scrBATCH_DYN;
