/* ?SetDesc@iaIACTOR@@QAAXV?$dsSMART_PTR@VdscDESC@@V?$_dsSP_OWNER_PLC_REFCOUNT@PAVdsREF_COUNT@@@@@@@Z @0x826D8010 */
#include "iaIACTOR.h"
#include "iaDESC.h"
#include "../dsc/dscDESC.h"

// iaIACTOR::SetDesc(dsSMART_PTR<dscDESC,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*> >) — 0x826D8010
// Adopt `spDescArg`'s referent as this actor's owned descriptor (releasing/replacing the
// previous one via smart-pointer assignment), then recompute `typeId`:
//   - if the new descriptor is non-empty, ask its runtime vtable for the concrete type id;
//   - otherwise fall back to the generic iaDESC::TYPE_ID constant.
// Finally release the caller's reference on the (by-value) argument.
void iaIACTOR::SetDesc(dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDescArg)
{
    spDesc = spDescArg;

    if (spDesc.pointee)
    {
        // Deviation from decompiler: it printed this call as
        // `pointee->__vftable[1].PostProcessPS(...)`, an aliasing artifact of the decompiler
        // reusing dscDESC_vtbl (5 slots, 20 bytes) for index arithmetic on a vtable that is
        // actually larger. DB-verified: the slot actually reached (vtable + 0x24) is
        // iaDESC_vtbl's TypeID (slot 9) — dscDESC_vtbl only has 5 slots and sslDESC_vtbl 9, so
        // TypeID is only introduced at the iaDESC level. Every dscDESC actually installed behind
        // an iaIACTOR's spDesc is (at least) an iaDESC, so this reinterpret is exact, not a guess.
        iaDESC *desc = (iaDESC *)spDesc.pointee;
        const iaDESC_vtbl *vtbl = (const iaDESC_vtbl *)desc->__vftable;
        vtbl->TypeID(desc, &typeId);
    }
    else
    {
        typeId = iaDESC::TYPE_ID;
    }

    spDescArg.DeletePointee();
}
