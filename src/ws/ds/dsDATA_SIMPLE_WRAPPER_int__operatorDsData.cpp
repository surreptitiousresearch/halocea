#include "dsDATA_SIMPLE_WRAPPER.h"

// dsDATA_SIMPLE_WRAPPER<int>::operator dsDATA() const @ 0x823B4510
// Box `val` into a type-erased dsDATA (adopts int's dsDATA_TYPE descriptor via StoreValue<T>).
// CAVEAT: same sret/this swap as the dsTSTRING<char> specialization (see that file) -- the
// decompiler names the hidden sret destination `this` and the real wrapper `this` "result".
template<>
dsDATA_SIMPLE_WRAPPER<int>::operator dsDATA() const
{
    dsDATA result;
    result.type = nullptr;
    result.StoreValue<int>(val);
    return result;
}
