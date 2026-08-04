// aiBASS::GetExFromStream @ 0x83243D68
// ?GetExFromStream@aiBASS@@MBAPBVexSTREAM_DATA@1@PAVaiBRAIN@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// The live exclamation record owned by `brain`, or null when there is none or it has expired
// (its owning brain has died).
const aiBASS::exSTREAM_DATA *aiBASS::GetExFromStream(aiBRAIN *brain) const
{
    ExStreamConstIter it = exStream.Find(&brain);
    if (it.IsDone())
        return nullptr;

    const exSTREAM_DATA *ex = &it.value();
    return ex->IsAIExpired() ? nullptr : ex;
}
