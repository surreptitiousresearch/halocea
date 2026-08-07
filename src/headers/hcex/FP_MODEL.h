#pragma once
/* FP_MODEL — one first-person bridge model: the ws-engine animation instance a local player's
 * view model is driven through, plus its per-camera visibility latches.
 *
 * DB-verified layout (types_members FP_MODEL, size 24): plrIdx@0 (int), modelId@4 (int),
 * modelIdx@8 (int), pInst@0x0C (animINST *), isActCamo@0x10 (int), isHiddenCam1@0x14 (bool),
 * isHiddenCam1Prev@0x15, isHiddenCam2@0x16, isHiddenCam2Prev@0x17 (bool).
 *
 * Stored in the global `fpModels`, a dsVECTOR<FP_MODEL,8> ({pData@0, nElem@4} per types_members);
 * src/headers/fp_model_vector.h spells that instantiation. */

typedef struct animINST animINST;   /* full layout: src/headers/animINST.h — pointer only here */

typedef struct FP_MODEL
{
    int       plrIdx;           /* 0x00 */
    int       modelId;          /* 0x04 */
    int       modelIdx;         /* 0x08 */
    animINST *pInst;            /* 0x0C */
    int       isActCamo;        /* 0x10 */
    bool      isHiddenCam1;     /* 0x14 */
    bool      isHiddenCam1Prev; /* 0x15 */
    bool      isHiddenCam2;     /* 0x16 */
    bool      isHiddenCam2Prev; /* 0x17 */
} FP_MODEL;                     /* 24 bytes */
