#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::AFF_PARAM::AFF_PARAM() @ 0x82A760D0
// Default-construct a blank affix parameter: both the `name` and `value` strings start out empty,
// each pointing at the process-wide shared empty-string singleton (bumping its refcount).
//
// CAVEAT: the disasm inlines dsTSTRING<char>::UnsafeInitEmpty for each member (clear pBuffer, lazily
// allocate the singleton on first use, copy its pBuffer, and increment the singleton's refCount).
// Modeled here as the two member default-constructions those inlined bodies implement.
dsAFFIX_BUILDER::AFF_PARAM::AFF_PARAM()
{
    this->name.pBuffer = nullptr;
    this->name.UnsafeInitEmpty();

    this->value.pBuffer = nullptr;
    this->value.UnsafeInitEmpty();
}
