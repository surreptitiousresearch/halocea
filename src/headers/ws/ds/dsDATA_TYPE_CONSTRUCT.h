#pragma once

namespace ds_data {

// Per-T storage-construction policy for the ds_data value model. For a heap-boxed type T the
// `storage` word holds a pointer to a heap T; these statics allocate / copy / free through it.
// GetPtr is the boundary allocator/accessor; Destroy/Copy/CopyObj are reversed per concrete T
// as explicit specializations in the corresponding .cpp files.
template<class T>
struct dsDATA_TYPE_CONSTRUCT {
    static void *GetPtr(const unsigned int *storage);              // boundary — box accessor/allocator
    static void  Destroy(unsigned int *storage);                  // free the boxed T, null the slot
    static void  Copy(const unsigned int *src, unsigned int *dest); // overwrite boxed T from another box
    static void  CopyObj(const T *obj, unsigned int *dest);        // overwrite boxed T from a raw object
};

} // namespace ds_data
