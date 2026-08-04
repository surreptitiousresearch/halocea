#include "../../headers/ws/ai/aiBRAIN_DESC_parse_boundary.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h" // aiSPD
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN_DESC::ReadSpeedList(const psSECTION&, const dsTSTRING<char>&) @ 0x83187A98
// Read the "speedList" sub-section: optional stay/taunt FSM events, then one ReadSpeed per named
// sub-section, followed by a validation pass ensuring maxDist strictly increases across entries
// (auto-correcting any that violate the ordering).
void aiBRAIN_DESC::ReadSpeedList(const psSECTION &psReadS, const dsTSTRING<char> &hintErr)
{
    const psSECTION_INTERFACE &psRead = static_cast<const psSECTION_INTERFACE &>(psReadS);
    if (!psRead.IsKeyExist("speedList", 0))
        return;

    psSECTION listSec = psRead.GetSection("speedList", 0);
    const psSECTION_INTERFACE &list = static_cast<const psSECTION_INTERFACE &>(listSec);

    if (list.IsKeyExist("stayFsmEvent", 0))
    {
        dsTSTRING<char> def;
        def.UnsafeInitEmpty();
        dsTSTRING<char> s = list.GetStr("stayFsmEvent", def, 0);
        if (s.pBuffer->strLen)
            stayFsmEvent = dsSTRID(s, false);
    }
    if (list.IsKeyExist("tauntFsmEvent", 0))
    {
        dsTSTRING<char> def;
        def.UnsafeInitEmpty();
        dsTSTRING<char> s = list.GetStr("tauntFsmEvent", def, 0);
        if (s.pBuffer->strLen)
            tauntFsmEvent = dsSTRID(s, false);
    }

    // One speed entry per named sub-section of "speedList".
    int nKey = list.GetNKey();
    for (int k = 0; k < nKey; ++k)
    {
        dsSTRID keyName = list.GetKeyName(k);
        const char *name = keyName.CStr();
        psSECTION sec = list.GetSection(name, 0);
        if (sec.pData)
        {
            dsTSTRING<char> nameStr(name);
            ReadSpeed(sec, nameStr, hintErr);
        }
    }

    // Validate: each usable entry's maxDist must exceed the previous usable entry's.
    int nElem = speedList.nElem;
    const char *prevName = "ZERO";
    float maxDist = 0.0f;
    for (int i = 0; i < nElem; ++i)
    {
        aiSPD &e = speedList[i];
        if (!e.name.IsValid() || !e.fsmEvent.IsValid())
        {
            _apLog("~AI,Error~%s: invalid speed: %s", hintErr.pBuffer->str, e.name.CStr());
            continue;
        }

        const char *name = e.name.CStr();

        bool usable;
        if (e.isForcedOnly)
        {
            usable = false;
        }
        else
        {
            float effMaxDist = e.tempMaxDist;
            if (effMaxDist < 0.0f)
                effMaxDist = e.maxDist;
            usable = effMaxDist > 0.0f;
        }

        if (usable && e.maxDist <= maxDist)
        {
            _apLog("~AI,Error~%s: invalid speed maxDist for %s, %.2f :",
                   hintErr.pBuffer->str, name, e.maxDist);
            _apLog("~AI,Error~   it must be greater than %.2f taken from %s", maxDist, prevName);
            e.maxDist = maxDist * 1.00001f;
            _apLog("~AI,Error~   Speed maxDist for %s is corrected to %.2f", name, e.maxDist);
        }

        maxDist = e.maxDist;
        prevName = name;
    }
}
