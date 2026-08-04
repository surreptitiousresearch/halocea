#pragma once
#include "hkVariant.h"

/* hkxAttribute (size 12, DB types_members) — a named, type-tagged value carried
   by an exported asset (the reflected object is reached through m_value). */
typedef struct hkxAttribute
{
    char *m_name;      /* 0 */
    hkVariant m_value; /* 4 (object + hkClass) */
} hkxAttribute;
