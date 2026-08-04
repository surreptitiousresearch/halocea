#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
#include "hkArray.h"

/* hkdAssetProcessingUtil::Output -- asset-processing output sink. The DB exposes
   no member layout for this type; only m_fractureTasks (offset 0, verified via
   disasm: lwz r6,0(output)) is read here. It points to the
   hkArray<hkdAssetProcessingUtil::FractureTasks> that queued static-fracture
   tasks are appended to. Remaining fields are unmodelled. */
typedef struct hkdAssetProcessingUtil_Output
{
    hkArray<> *m_fractureTasks; /* 0 */
} hkdAssetProcessingUtil_Output;
