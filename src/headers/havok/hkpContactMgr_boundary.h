#pragma once
#include "hkReferencedObject.h"
#include "hkpContactMgrFactory.h"

typedef struct hkpWorld hkpWorld;
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;

/* DB exposes these enumerator / vtable-symbol NAMES, not values/addresses. */
extern const int HK_MEMORY_CLASS_CONTACT;
extern const int HK_MEMORY_CLASS_DYNAMICS;
extern hkBaseObject_vtbl hkpNullContactMgrFactory_vftable; /* hkpNullContactMgrFactory::`vftable' */
extern hkBaseObject_vtbl hkpNullContactMgr_vftable;        /* hkpNullContactMgr::`vftable' */

hkpContactMgrFactory *hkpSimpleConstraintContactMgr_Factory_construct(void *chunk);        /* hkpSimpleConstraintContactMgr::Factory::Factory */
hkpContactMgrFactory *hkpReportContactMgr_Factory_construct(void *chunk, hkpWorld *world); /* hkpReportContactMgr::Factory::Factory */
void hkpCollisionDispatcher_registerContactMgrFactoryWithAll(hkpCollisionDispatcher *dispatcher, hkpContactMgrFactory *factory, int type);
