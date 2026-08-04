#pragma once
#include "hkReferencedObject.h"

/* hkdAction, size 8 — just an hkReferencedObject base. Concrete actions (e.g.
   hkdDecorateFractureFaceAction) embed this as their first sub-object. */
typedef struct hkdAction
{
    hkReferencedObject base; /* 0 */
} hkdAction;
