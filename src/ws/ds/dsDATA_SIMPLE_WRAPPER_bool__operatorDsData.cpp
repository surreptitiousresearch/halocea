#include "dsDATA_SIMPLE_WRAPPER.h"

// dsDATA_SIMPLE_WRAPPER<bool>::operator dsDATA() const @ 0x823D3C48
// Box `val` into a type-erased dsDATA (adopts bool's dsDATA_TYPE descriptor via StoreValue<T>).
// CAVEAT: same sret/this swap as the dsTSTRING<char> specialization (see that file).
template<>
dsDATA_SIMPLE_WRAPPER<bool>::operator dsDATA() const
{
    dsDATA result;
    result.type = nullptr;
    result.StoreValue<bool>(val);
    return result;
}
