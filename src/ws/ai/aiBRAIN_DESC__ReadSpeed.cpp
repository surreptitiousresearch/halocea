#include "../../headers/ws/ai/aiBRAIN_DESC_parse_boundary.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiSPD
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN_DESC::ReadSpeed(const psSECTION&, const dsTSTRING<char>&, const dsTSTRING<char>&)
// @ 0x83188910
// Parse a single named speed/gait entry and upsert it into speedList. A new entry is seeded from
// any existing entry of the same name so unspecified fields inherit. The fsmTriggerEventOn/Off
// pair must be given both-or-none.
void aiBRAIN_DESC::ReadSpeed(const psSECTION &psRead, const dsTSTRING<char> &name,
                             const dsTSTRING<char> &hintErr)
{
    aiSPD spd;
    dsSTRID nameId(name, false);

    // Seed from an existing same-named entry (so ParsePS only overrides specified fields).
    for (int i = 0; i < speedList.nElem; ++i)
    {
        aiSPD &existing = speedList[i];
        if (existing.name.IsValid() && nameId.id == existing.name.id)
        {
            spd = existing;
            break;
        }
    }

    spd.ParsePS(&psRead, &name);

    if (!spd.name.IsValid() || !spd.fsmEvent.IsValid())
    {
        _apLog("~AI,Error~%s: can't read speed - no fsmEvent or speed name!",
               hintErr.pBuffer->str);
        return;
    }

    bool hasTriggerOn  = !spd.fsmTriggerEventOn.IsEmpty();
    bool hasTriggerOff = !spd.fsmTriggerEventOff.IsEmpty();
    if (hasTriggerOn != hasTriggerOff)
    {
        _apLog("~AI,Error~%s: can't read speed - fsmTriggerEventOn & fsmTriggerEventOff "
               "must be described both or none!", hintErr.pBuffer->str);
        return;
    }

    // Upsert: overwrite a same-named entry in place, else append.
    int n = speedList.nElem;
    int j = 0;
    for (; j < n; ++j)
    {
        aiSPD &existing = speedList[j];
        if (existing.name.IsValid() && spd.name.id == existing.name.id)
        {
            existing = spd;
            break;
        }
    }
    if (j >= n)
        speedList.PushBack(spd);
}
