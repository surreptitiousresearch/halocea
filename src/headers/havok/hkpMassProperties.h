#pragma once
#include "hkVector4.h"
#include "hkMatrix3.h"

/* hkpMassProperties, size 80. Verified against DB types_members. */
typedef struct hkpMassProperties
{
    float m_volume;                 /* 0  */
    float m_mass;                   /* 4  */
    unsigned char _pad08[8];        /* 8  (alignment before the vector) */
    hkVector4 m_centerOfMass;       /* 16 */
    hkMatrix3 m_inertiaTensor;      /* 32 (size 48) -> 80 */
} hkpMassProperties;
