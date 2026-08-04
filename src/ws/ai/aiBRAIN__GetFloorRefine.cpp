#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/mdl/mdlCDT_REFINE_FLOOR_AI.h"
#include "../../headers/ws/cdt/cdtREFINE.h" // pInstSelf

// aiBRAIN::GetFloorRefine() const @ 0x832AC8B0
// ?GetFloorRefine@aiBRAIN@@UBAPAVcdtREFINE@@XZ
//
// Return the process-wide AI floor collision-refine query object (lazily constructed on first use),
// after pointing its self-exclusion instance at this brain's animated instance.
mdlCDT_REFINE_FLOOR_AI *aiBRAIN::GetFloorRefine() const
{
    static mdlCDT_REFINE_FLOOR_AI refine(nullptr);
    refine.pInstSelf = this->pInst;
    return &refine;
}
