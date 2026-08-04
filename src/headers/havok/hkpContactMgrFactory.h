#pragma once
#include "hkReferencedObject.h"

typedef struct hkpContactMgrFactory_vtbl hkpContactMgrFactory_vtbl;

/* size 8 (bare hkReferencedObject); concrete factories extend it. */
typedef struct hkpContactMgrFactory
{
    hkReferencedObject base;
} hkpContactMgrFactory;
