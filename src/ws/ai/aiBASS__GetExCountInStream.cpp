// aiBASS::GetExCountInStream @ 0x83244070
// ?GetExCountInStream@aiBASS@@MBAHABVdsSTRID@@@Z
#include "../../headers/ws/ai/aiBASS.h"

// Count of non-expired stream entries whose name matches `soundID`. const method walking the
// non-const map (const_cast per corpus convention).
int aiBASS::GetExCountInStream(const dsSTRID &soundID) const
{
    int count = 0;
    ExStreamMap &stream = const_cast<ExStreamMap &>(exStream);
    for (ExStreamIter it(stream.Begin()); !it.IsDone(); ++it)
    {
        exSTREAM_DATA *entry = it.value();
        if (entry->name.id == soundID.id && !entry->IsExExpired())
            ++count;
    }
    return count;
}
