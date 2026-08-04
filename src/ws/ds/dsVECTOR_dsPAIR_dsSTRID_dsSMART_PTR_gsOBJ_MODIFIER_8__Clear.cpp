#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::Clear @ 0x826CCA98
// Release every live pair's smart-pointer value (dropping its reference and deleting the pointee
// at zero), then reset the count. The dsSTRID key is a trivial interned-id pointer, no teardown.
template<>
void dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                          _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index)
        this->pData[index].val.DeletePointee();
    this->nElem = 0;
}
