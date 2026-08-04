#include "../headers/havok/hkReferencedObject.h"

typedef struct hkClass hkClass;

/* hkReferencedObject::getClassType — base implementation returns null; concrete
   subclasses override to return their static reflection hkClass. */
const hkClass *hkReferencedObject_getClassType(hkReferencedObject *self)
{
    (void)self;
    return 0;
}
