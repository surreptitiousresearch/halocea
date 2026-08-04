#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::_internalRealloc @ 0x826CB3A8
// Element size 8 bytes (dsSTRID interned-id pointer key + dsSMART_PTR pointer value).
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                                _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > > *)
        dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
