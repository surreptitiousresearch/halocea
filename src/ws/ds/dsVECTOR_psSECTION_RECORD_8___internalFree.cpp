#include "dsVECTOR.h"

struct psSECTION_RECORD; // boundary — ps (particle system) section record (element by value)

extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<psSECTION_RECORD,8>::_internalFree @ 0x82516698
// Free a backing-storage block. psSECTION_RECORD's per-element teardown (value.type->Destroy)
// happens before this is called (see the destructor); this is a bare dlFree of the block.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::_internalFree(void *p)
{
    dlFree(p);
}
