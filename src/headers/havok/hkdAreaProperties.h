#pragma once
/* hkdAreaProperties (16 bytes) — DB types_members: four floats. */
typedef struct hkdAreaProperties
{
    float m_area;            /* 0  */
    float m_phi;             /* 4  */
    float m_sectionModulusY; /* 8  */
    float m_sectionModulusZ; /* 12 */
} hkdAreaProperties;
