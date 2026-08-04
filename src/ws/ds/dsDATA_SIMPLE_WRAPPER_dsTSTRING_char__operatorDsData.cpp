#include "dsDATA_SIMPLE_WRAPPER.h"
#include "dsTSTRING.h"

// dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>>::operator dsDATA() const @ 0x823B44D8
// Box `val` into a type-erased dsDATA (adopts dsTSTRING<char>'s dsDATA_TYPE descriptor via
// StoreValue<T>).
// CAVEAT (sret/this swap, same pattern as sslOBJ_REF::operator dsDATA -- see that file): the
// decompiler names the hidden sret destination pointer `this` and the real `this` (the wrapper)
// `result`, because dsDATA is 8 bytes and returns via a hidden pointer in r3, with the real
// `this` in r4. Reconstructed with the real signature/semantics.
template<>
dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>>::operator dsDATA() const
{
    dsDATA result;
    result.type = nullptr;
    result.StoreValue<dsTSTRING<char>>(val);
    return result;
}
