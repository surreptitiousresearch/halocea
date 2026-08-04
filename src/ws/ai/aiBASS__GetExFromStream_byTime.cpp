// aiBASS::GetExFromStream(float) @ 0x83243F30
// ?GetExFromStream@aiBASS@@MBAPBVexSTREAM_DATA@1@M@Z
#include "../../headers/ws/ai/aiBASS.h"

// First stream entry that has been running no longer than `maxTimeFromStart` and is not expired,
// else null. const method walking the non-const map (const_cast per corpus convention).
const aiBASS::exSTREAM_DATA *aiBASS::GetExFromStream(float maxTimeFromStart) const
{
    ExStreamMap &stream = const_cast<ExStreamMap &>(exStream);
    for (ExStreamIter it(stream.Begin()); !it.IsDone(); ++it)
    {
        exSTREAM_DATA *entry = it.value();
        if (entry->time <= maxTimeFromStart && !entry->IsExpired())
            return entry;
    }
    return nullptr;
}
