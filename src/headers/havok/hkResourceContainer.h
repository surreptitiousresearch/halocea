#pragma once
#include "hkClass.h"
#include "hkReferencedObject.h"

/* Havok serialization resource graph (DB types_members bodies below).
   All are virtual classes whose vtable pointer sits at offset 0 (inside the
   hkReferencedObject/hkBaseObject base); the calls below are the virtual
   slots this subsystem uses:
     hkResourceContainer::getName           vtable +0x10
     hkResourceHandle::getName              vtable +0x10
     hkResourceHandle::getObject            vtable +0x18
     hkResourceContainer::findResourceByName  vtable +0x20
     hkResourceContainer::getParent           vtable +0x24
     hkResourceContainer::findContainerByName vtable +0x34 */

/* hkResourceBase (8 bytes, DB: hkReferencedObject base only). */
struct hkResourceBase : hkReferencedObject
{
};
typedef struct hkResourceBase hkResourceBase;

/* hkResourceHandle (8 bytes, DB: hkResourceBase base only — abstract). */
struct hkResourceHandle : hkResourceBase
{
};
typedef struct hkResourceHandle hkResourceHandle;

/* hkResourceContainer (8 bytes, DB: hkResourceBase base only — abstract). */
struct hkResourceContainer : hkResourceBase
{
};
typedef struct hkResourceContainer hkResourceContainer;

/* hkClass reflection descriptors used to select resources by type. */
extern const hkClass hkpRigidBodyClass;
extern const hkClass hkpShapeInfoClass;

const char *hkResourceContainer_getName(hkResourceContainer *self, char *nameBuffer);
const char *hkResourceHandle_getName(const hkResourceHandle *self, char *nameBuffer);
void *hkResourceHandle_getObject(hkResourceHandle *self);
hkResourceHandle *hkResourceContainer_findResourceByName(hkResourceContainer *self,
                                                         const char *name,
                                                         const hkClass *type,
                                                         void *reserved);
hkResourceContainer *hkResourceContainer_getParent(hkResourceContainer *self);
hkResourceContainer *hkResourceContainer_findContainerByName(hkResourceContainer *self,
                                                             const char *name,
                                                             hkResourceContainer *previous);
