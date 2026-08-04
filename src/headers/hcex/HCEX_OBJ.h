#pragma once
/* HCEX_OBJ — one entry in the hcexObjects sorted vector: maps a Blam object id to its ws-engine
 * animation instance and follower list. 56 bytes; layout from the database (types_members HCEX_OBJ),
 * including the DB's 32-bit bitfield block at 0x34 (17 reserved bits then the isFunc* / isMachine* /
 * isShieldShow/isActCamo/isHidden/isInited flags). The dsVECTOR<...,8> members are ws-engine C++
 * containers — a generic boundary shim template reproduces their 20-byte layout here.
 *
 * Consumers that need the verified MASK semantics (the DB bitfield declaration order is not a
 * reliable guide to the PPC bit positions) access the whole 32-bit block via HCEX_OBJ_FLAGS(). */

typedef struct animINST animINST;
typedef struct entENTITY entENTITY;   /* ws entity — boundary */
typedef struct m3dMATR m3dMATR;       /* ws 4x4 matrix — boundary */

#include "../apCL.h"

/* ds::WEAK_PTR<T> — boundary shim (referenced only as a dsVECTOR element type here). */
template<class T>
struct WEAK_PTR
{
    void *_opaque; /* boundary */
};

/* dsVECTOR<T,N> — 20-byte ws vector header. Use the canonical template (identical layout:
 * pData@0, nElem@4, allocated@8, __cl@12) rather than a local shim, to avoid a C2953 duplicate
 * definition when this header and ws/ds/dsVECTOR.h are pulled into the same TU. */
#include "../ws/ds/dsVECTOR.h"

/* Legacy flattened spellings kept for existing callers. */
typedef dsVECTOR<WEAK_PTR<entENTITY>, 8> dsVECTOR_followers;
typedef dsVECTOR<m3dMATR, 8> dsVECTOR_followersOfs;

typedef struct HCEX_OBJ
{
    int                  id;            /* 0x00 */
    animINST            *pInst;         /* 0x04 */
    int                  modelIdx;      /* 0x08 */
    dsVECTOR<WEAK_PTR<entENTITY>, 8> followers;  /* 0x0C */
    dsVECTOR<m3dMATR, 8> followersOfs;  /* 0x20 */
    unsigned __int32 _reserved_bits : 17; /* 0x34 (anonymous :17 in DB) */
    unsigned __int32 isFuncDin : 1;
    unsigned __int32 isFuncCin : 1;
    unsigned __int32 isFuncBin : 1;
    unsigned __int32 isFuncAin : 1;
    unsigned __int32 isFuncD : 1;
    unsigned __int32 isFuncC : 1;
    unsigned __int32 isFuncB : 1;
    unsigned __int32 isFuncA : 1;
    unsigned __int32 isMachinePositionOn : 1;
    unsigned __int32 isMachinePowerOn : 1;
    unsigned __int32 isMachineLocked : 1;
    unsigned __int32 isShieldShow : 1;
    unsigned __int32 isActCamo : 1;
    unsigned __int32 isHidden : 1;
    unsigned __int32 isInited : 1;
} HCEX_OBJ;                             /* 56 bytes */

/* Whole 32-bit flag block at 0x34 — use with the verified masks below. */
#define HCEX_OBJ_FLAGS(obj) (*(unsigned int *)((char *)(obj) + 0x34))

/* isInited = bit 25 (0x2000000). Verified against the binary: hcex_obj_init @0x823EF140 sets it with
 * `oris r7,r7,0x200` (0x200<<16). Do NOT "correct" this to bit 0 — the DB member order is not a reliable
 * guide to the actual bit position for this unit. */
#define HCEX_OBJ_FLAG_IS_INITED 0x2000000u  /* bit 25 (isInited) */

/* isHidden = bit 30 (0x40000000). Verified against hcex_hide_obj_follow_hier @0x823B2BE8, which sets/clears
 * exactly this bit of `flags` to reflect the cinematic hidden state. */
#define HCEX_OBJ_FLAG_IS_HIDDEN 0x40000000u  /* bit 30 (isHidden) */
