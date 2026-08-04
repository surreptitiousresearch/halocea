// aiBASS::RemoveExFromStream @ 0x83245760
// ?RemoveExFromStream@aiBASS@@MAAXPBVexSTREAM_DATA@1@@Z
#include "../../headers/ws/ai/aiBASS.h"

// Mute and drop the stream entry belonging to `ex`'s owning brain. No-op when the owner has already
// died (weak handle nulled) or has no entry in the stream.
void aiBASS::RemoveExFromStream(const exSTREAM_DATA *ex)
{
    ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = ex->owner.pHandle;
    aiBRAIN *brain = handle ? handle->pPtr : nullptr;
    if (!brain)
        return;

    ExStreamIter it = exStream.Find(&brain);
    if (!it.IsDone())
    {
        InternalMuteSound(it.value());
        exStream.Erase(it);
    }
}
