#pragma once
#include "hkTransform.h"
#include "hkVector4.h"
#include "hkBool.h"

/* Boundary types for hkdConnectivityCalculator (2-level descent). Layouts are
   DB-verified (types_members). The calculator itself is not reversed here. */

typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
typedef struct hkpShape hkpShape;

/* hkdConnectivityCalculator::ConnectivityInput (DB types_members). */
typedef struct hkdConnectivityCalculator_ConnectivityInput
{
    hkpCollisionDispatcher *m_dispatcher;  /* 0   */
    const hkpShape *m_shapeA;              /* 4   */
    const hkpShape *m_shapeB;              /* 8   */
    hkTransform m_transformA;              /* 16  */
    hkTransform m_transformB;              /* 80  */
    float m_maxDistance;                   /* 144 */
    hkBool m_simplifyComplexShapeBByPlane; /* 148 */
} hkdConnectivityCalculator_ConnectivityInput;

/* hkdConnectivityCalculator::ConnectivityOutput (DB types_members). */
typedef struct hkdConnectivityCalculator_ConnectivityOutput
{
    hkVector4 m_separatingNormalA; /* 0  */
    hkVector4 m_pivotA;            /* 16 */
    float m_area;                  /* 32 */
    float m_distance;              /* 36 */
    float m_phi;                   /* 40 */
    float m_sectionModulusY;       /* 44 */
    float m_sectionModulusZ;       /* 48 */
} hkdConnectivityCalculator_ConnectivityOutput;

extern int hkdConnectivityCalculator_calculateSemiAccurateConnectivity(
    hkdConnectivityCalculator_ConnectivityInput *input,
    hkdConnectivityCalculator_ConnectivityOutput *output); /* returns hkBool (byte) */
