#pragma once
#include "hkVector4.h"
#include "hkRefPtr.h"

typedef struct hkdGeometry hkdGeometry;

/* hkdWoodFracture::SplittingData (DB layout; gaps left as reserved). */
typedef struct hkdWoodFracture_SplittingData
{
    hkRefPtr<hkdGeometry> m_splitGeom;    /* 0  */
    unsigned char _pad04[12];             /* 4  */
    hkVector4 m_splittingAxis;            /* 16 */
    float m_numSubparts;                  /* 32 */
    float m_widthRange;                   /* 36 */
    unsigned char _pad28[8];              /* 40 */
    hkVector4 m_scale;                    /* 48 */
    hkVector4 m_scaleRange;               /* 64 */
    float m_splitGeomShiftRangeY;         /* 80 */
    float m_splitGeomShiftRangeZ;         /* 84 */
    float m_surfaceNormalShearingRange;   /* 88 */
    float m_fractureLineShearingRange;    /* 92 */
    float m_fractureNormalShearingRange;  /* 96 */
} hkdWoodFracture_SplittingData;
