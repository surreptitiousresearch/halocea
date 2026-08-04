#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsSTRID.h"
#include "dsSMART_PTR.h"
#include "dsREF_COUNT.h"

struct gsOBJ_MODIFIER_BASE_DESC; // boundary — gs subsystem object-modifier descriptor (pointee only)

// dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,...>>,8>::Length @ 0x826CAD58
template<>
int dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC,
                                         _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8>::Length() const
{
    return this->nElem;
}
