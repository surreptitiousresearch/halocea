/* cleanupLoadedObjecthkdDecorateFractureFaceActionFractureFaceInfo @0x838319C8 */
#include "../headers/havok/hkReferencedObject.h"

/* cleanup thunk: drop the held reference (if any) and clear the slot. */
void cleanupLoadedObjecthkdDecorateFractureFaceActionFractureFaceInfo(hkReferencedObject **p)
{
    hkReferencedObject *object = *p;
    if (object)
        hkReferencedObject_removeReference(object);
    *p = 0;
}
