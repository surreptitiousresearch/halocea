/* ?getClassType@hkReferencedObject@@UBAPBVhkClass@@XZ @0x838D2AF0 */
#include "../headers/havok/hkReferencedObject.h"

typedef struct hkClass hkClass;

/* hkReferencedObject::getClassType — base implementation returns null; concrete
   subclasses override to return their static reflection hkClass. */
const hkClass *hkReferencedObject_getClassType(hkReferencedObject *self)
{
    (void)self;
    return 0;
}
