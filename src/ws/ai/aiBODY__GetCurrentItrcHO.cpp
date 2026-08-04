#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/use/usePROCESSOR.h" // usePROCESSOR::hoInfo
#include "../../headers/ws/fsm/fsmHO_INFO.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h" // mdlITRC_BASE::IsHO / IsValid
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/Deleter.h"

// aiBODY::GetCurrentItrcHO(aiBODY_GET_ITRCHO) const @ 0x832ACDB0
// ?GetCurrentItrcHO@aiBODY@@UBA?AV?$dsSHARED_PTR@VmdlITRC_BASE@@$0A@V?$Deleter@VmdlITRC_BASE@@@@@@W4aiBODY_GET_ITRCHO@aiBODY_IFACE@@@Z
//
// Return the body's current hideout ITRC. Prefer the HO itrc when it is a live hideout, else fall
// back to the common itrc. The result is empty unless the chosen itrc passes the filter/IsValid
// gate (filter != VALID_ONLY skips the validity check).
dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> >
aiBODY::GetCurrentItrcHO(aiBODY_GET_ITRCHO filter) const
{
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > result;
    if (!useProcessor)
        return result;

    fsmHO_INFO &ho = useProcessor->hoInfo;
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > &chosen =
        (ho.itrcHo.pointee && ho.itrcHo.pointee->IsHO()) ? ho.itrcHo : ho.itrcCommon;

    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > cur = chosen;
    if (cur.pointee && (static_cast<int>(filter) != 0 || cur.pointee->IsValid()))
        result = cur;
    return result;
}
