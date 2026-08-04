#pragma once
#include "hkBool.h"
#include "hkRotation.h"
#include "hkdShapeInstanceInfo.h"

/* hkdWoodFracture::SplitParams (DB layout, size 80).
   hkdBreakableShape is forward-declared in hkdShapeInstanceInfo.h. */
typedef struct hkdFracture_FractureInput hkdFracture_FractureInput; /* hkdFracture::FractureInput */
typedef hkdFracture_FractureInput FractureInput;
typedef struct hkPseudoRandomGenerator hkPseudoRandomGenerator;

typedef struct hkdWoodFracture_SplitParams
{
    const FractureInput *m_fractureInput;                    /* 0  */
    hkPseudoRandomGenerator *m_randomGenerator;              /* 4  */
    hkArray<hkdShapeInstanceInfo> *m_splitShapesOut;         /* 8  */
    const hkdBreakableShape *m_rootBreakableShape;           /* 12 */
    hkBool m_addNewShapesToParents;                          /* 16 */
    unsigned char _pad11[15];                                /* 17 pad to 32-align */
    hkRotation m_splittingTransform;                         /* 32 */
} hkdWoodFracture_SplitParams;
