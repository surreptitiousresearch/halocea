#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/use/usePROCESSOR.h" // usePROCESSOR::hoInfo
#include "../../headers/ws/fsm/fsmHO_INFO.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h" // mdlITRC_BASE::IsHO

// aiBODY::GetFsmInfo(fsmHO_INFO&) @ 0x832AD500
// ?GetFsmInfo@aiBODY@@UAA_NAAUfsmHO_INFO@@@Z
//
// Copy the use processor's current hideout info into `infoHO`. True only when the HO itrc exists and
// reports itself as an active hideout.
bool aiBODY::GetFsmInfo(fsmHO_INFO &infoHO)
{
    if (!useProcessor)
        return false;

    infoHO = useProcessor->hoInfo;
    if (!infoHO.itrcHo.pointee)
        return false;
    if (!infoHO.itrcHo.pointee->IsHO())
        return false;
    return true;
}
