/* cleanupLoadedObjecthkpBoxMotion @0x838F41F8 */
#include "../headers/havok/havok_cleanup_thunk.h"

/* cleanupLoadedObject<T> instantiation: run the object's virtual (deleting)
   destructor in place, leaving the backing store for the loader to reclaim. */
void cleanupLoadedObjecthkpBoxMotion(hk_loaded_object *object)
{
    object->vtable[0](object, 0);
}
