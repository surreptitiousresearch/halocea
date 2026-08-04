#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::_internalFree @ 0x826CB640
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::_internalFree(void *p)
{
    dlFree(p);
}
