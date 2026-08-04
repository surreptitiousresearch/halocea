#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */

/* Target shape for the cleanupLoadedObject<T> thunks.

   A serialized/loaded Havok object begins with a vtable pointer. Vtable slot 0
   is the virtual (deleting) destructor, signature void(*)(self, int deleting).
   Each cleanupLoadedObjecthkd<T> is a template instantiation that loads the
   object's vtable and tail-calls slot 0 with deleting=0 — i.e. run the
   destructor in place but do NOT free the backing store (the loader owns it).

   The concrete T is erased in every instantiation (the body is byte-identical
   across all of them), so a single generic object/vtable shape models them all. */
typedef struct hk_loaded_object hk_loaded_object;
typedef void (__fastcall *hk_deleting_destructor)(hk_loaded_object *self, int deleting);

struct hk_loaded_object
{
    hk_deleting_destructor *vtable; /* vtable[0] = deleting destructor */
};
