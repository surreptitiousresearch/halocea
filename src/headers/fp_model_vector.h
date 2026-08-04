#pragma once
/* fp_model_vector — dsVECTOR<FP_MODEL,8>: first-person bridge models. */

#include "animINST.h"

typedef struct FP_MODEL   /* 24 bytes; layout from the database (types_members) */
{
    int           plrIdx;           /* 0x00 */
    int           modelId;          /* 0x04 */
    int           modelIdx;         /* 0x08 */
    animINST     *pInst;            /* 0x0C */
    int           isActCamo;        /* 0x10 */
    bool          isHiddenCam1;     /* 0x14 */
    bool          isHiddenCam1Prev; /* 0x15 */
    bool          isHiddenCam2;     /* 0x16 */
    bool          isHiddenCam2Prev; /* 0x17 */
} FP_MODEL;

/* DB dsVECTOR<FP_MODEL,8> via the canonical template (the former flat model had pData/nElem
 * swapped relative to the DB layout: pData@0, nElem@4). */
#include "ws/ds/dsVECTOR.h"
typedef dsVECTOR<FP_MODEL, 8> fp_model_vector;
