#pragma once
#include "hkRotation.h"
#include "hkVector4.h"
/* size 64 */
typedef struct hkTransform
{
    hkRotation m_rotation;   /* 0  */
    hkVector4 m_translation; /* 48 */
} hkTransform;
