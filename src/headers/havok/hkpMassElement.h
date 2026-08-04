#pragma once
#include "hkpMassProperties.h"
#include "hkTransform.h"

/* hkpMassElement (size 144, DB types_members) — one child's mass properties in a
   given placement. An array of these is fed to
   hkpInertiaTensorComputer::combineMassProperties to accumulate a parent's mass
   properties from its children. */
typedef struct hkpMassElement
{
    hkpMassProperties m_properties; /* 0  (size 80) */
    hkTransform m_transform;        /* 80 (size 64) -> 144 */
} hkpMassElement;
