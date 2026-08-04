#include "dsDATA.h"
#include "dsDATA_TYPE.h"

// dsDATA::dsDATA(const dsDATA_TYPE*) @ 0x82516B70
// Construct an empty value of `_type` in place: adopt the type descriptor, then dispatch its
// Construct virtual (vtable slot 0) to default-construct the value into `storage`.
// DEVIATION: the decompiler rendered the virtual dispatch as `_type->Construct(_type,
// &this->storage)`, showing `_type` twice (once as the shown callee object, once as an
// apparent first argument) -- that's the decompiler's usual artifact of exposing the implicit
// `this` of a const-qualified vtable call as a visible argument. Real call takes one argument.
dsDATA::dsDATA(const dsDATA_TYPE *_type)
{
    this->type = _type;
    _type->Construct(&this->storage);
}
