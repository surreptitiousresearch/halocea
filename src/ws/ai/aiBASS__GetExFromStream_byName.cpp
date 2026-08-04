// aiBASS::GetExFromStream(const dsSTRID&) @ 0x83243DF8
// ?GetExFromStream@aiBASS@@MBAPBVexSTREAM_DATA@1@ABVdsSTRID@@@Z
#include "../../headers/ws/ai/aiBASS.h"

// First non-expired stream entry whose name matches `soundID`, else null. The method is const but
// walks the (non-const) map — mirror that access with a const_cast, matching the established
// aiCOORDINATOR::CalcPenaltyFromUsedWPs convention.
const aiBASS::exSTREAM_DATA *aiBASS::GetExFromStream(const dsSTRID &soundID) const
{
    ExStreamMap &stream = const_cast<ExStreamMap &>(exStream);
    for (ExStreamIter it(stream.Begin()); !it.IsDone(); ++it)
    {
        exSTREAM_DATA *entry = it.value();
        if (entry->name.id == soundID.id && !entry->IsExExpired())
            return entry;
    }
    return nullptr;
}
