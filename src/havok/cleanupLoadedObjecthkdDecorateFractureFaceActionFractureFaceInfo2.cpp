/* cleanupLoadedObjecthkdDecorateFractureFaceActionFractureFaceInfo2 @0x83831A08 */
#include "../headers/havok/hkReferencedObject.h"

/* cleanup thunk: drop the held reference (if any) and clear the slot. */
void cleanupLoadedObjecthkdDecorateFractureFaceActionFractureFaceInfo2(hkReferencedObject **p)
{
    hkReferencedObject *object = *p;
    if (object)
        hkReferencedObject_removeReference(object);
    *p = 0;
}
