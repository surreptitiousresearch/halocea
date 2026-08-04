#pragma once
#include "hkpCollisionInput.h"
#include "hkVector4.h"

typedef struct hkpCollisionAgentConfig hkpCollisionAgentConfig;

/* hkpLinearCastCollisionInput, size 112. Extends hkpCollisionInput with the
   cast path and the cached path length written by the caller. */
typedef struct hkpLinearCastCollisionInput
{
    hkpCollisionInput base;               /* 0   */
    hkVector4 m_path;                     /* 80  */
    float m_maxExtraPenetration;          /* 96  */
    float m_cachedPathLength;             /* 100 */
    hkpCollisionAgentConfig *m_config;    /* 104 */
} hkpLinearCastCollisionInput;
