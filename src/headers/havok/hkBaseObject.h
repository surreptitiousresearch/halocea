#pragma once

typedef struct hkBaseObject_vtbl hkBaseObject_vtbl;

/* size 4 — the vtable pointer; base of every polymorphic Havok object. */
typedef struct hkBaseObject
{
    hkBaseObject_vtbl *__vftable;
} hkBaseObject;
