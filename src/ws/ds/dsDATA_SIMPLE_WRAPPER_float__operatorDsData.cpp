#include "dsDATA_SIMPLE_WRAPPER.h"

// dsDATA_SIMPLE_WRAPPER<float>::operator dsDATA() const @ 0x823EAF00
// Box `val` into a type-erased dsDATA (adopts float's dsDATA_TYPE descriptor via StoreValue<T>).
// CAVEAT: same sret/this swap as the dsTSTRING<char> specialization (see that file); here the
// decompiler additionally shows the source read as `&result->storage` (mistyping the wrapper's
// `this` as a `dsDATA*`) -- offset 0 of that mistyped view is the same address as `val`
// (dsDATA::storage and dsDATA_SIMPLE_WRAPPER<float>::val are both the type's sole member at
// offset 0), so this collapses to `&val`.
template<>
dsDATA_SIMPLE_WRAPPER<float>::operator dsDATA() const
{
    dsDATA result;
    result.type = nullptr;
    result.StoreValue<float>(val);
    return result;
}
