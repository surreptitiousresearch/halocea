#pragma once
#include "hkClass.h"
/* size 8 — a type-tagged pointer (object + its reflected class). */
typedef struct hkVariant
{
    void *m_object;         /* offset 0 */
    const hkClass *m_class; /* offset 4 */
} hkVariant;
