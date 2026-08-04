#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ds/dsSTRID.h"

// aiBASS::FilterExVsStream(aiBRAIN*, const exDATA&, float) @ 0x83240548
// ?FilterExVsStream@aiBASS@@MAA_NPAVaiBRAIN@@ABVexDATA@1@M@Z
//
// Gate an incoming exclamation for `pBrain` at play distance `dist` against the currently in-flight
// per-brain stream, applying the category's overlap policy:
//   DELAY   — not supported; always cancelled.
//   REPLACE — honoured only if the exMaxCount budget is free AND (there is no existing entry for the
//             brain that already outranks the incoming, or the stream is full and no replaceable
//             victim exists).
//   SKIP    — honoured only if the stream is not full, the brain is not already speaking, no same
//             group entry is queued (when the category is unlimited), and no just-started sim sound
//             is playing.
// Returns true when the exclamation survives the filter and may be enqueued.
//
// NOTE: the decompiler failed local-variable allocation here and lowered every helper call as a raw
// __vftable dispatch on aiBASS's own manual vtable. Each such slot is re-expressed as its named
// method (uniquely identified by signature): +60 IsStreamFull, +64/+68/+72 the GetExFromStream
// overloads, +80/+84 the TryToReplace overloads. The spurious `0` arg the decompiler attached to the
// REPLACE-branch IsStreamFull call is a failed-alloc artifact (IsStreamFull takes no args).
bool aiBASS::FilterExVsStream(aiBRAIN *pBrain, const exDATA &exclamation, float dist)
{
    if (exclamation.overlap != exOVERLAP_SKIP)
    {
        if (exclamation.overlap == exOVERLAP_DELAY)
        {
            _apLog("~BASS~Exclamation (DELAY) cancelled - not supporter for now");
            return false;
        }
        if (exclamation.overlap == exOVERLAP_REPLACE)
        {
            if (exclamation.exMaxCount >= 0
                && GetExCountInStream(exclamation.name) >= exclamation.exMaxCount)
            {
                _apLog("~BASS~Exclamation (REPLACE) cancelled - exMaxCount reached");
                return false;
            }

            const exSTREAM_DATA *existing = GetExFromStream(pBrain);
            if (existing)
            {
                if (!TryToReplace(existing, exclamation, dist))
                {
                    _apLog("~BASS~Exclamation (REPLACE) cancelled - brain already have better");
                    return false;
                }
            }
            else if (IsStreamFull() && !TryToReplace(exclamation, dist))
            {
                _apLog("~BASS~Exclamation (REPLACE) cancelled - stream already have better");
                return false;
            }
            return true;
        }
        // Unknown / invalid overlap policy.
        return false;
    }

    // exOVERLAP_SKIP
    if (exclamation.exMaxCount >= 0
        && GetExCountInStream(exclamation.name) >= exclamation.exMaxCount)
    {
        _apLog("~BASS~Exclamation (SKIP) cancelled - exMaxCount reached");
        return false;
    }
    if (IsStreamFull())
    {
        _apLog("~BASS~Exclamation (SKIP) cancelled - stream is full");
        return false;
    }
    if (GetExFromStream(pBrain))
    {
        _apLog("~BASS~Exclamation (SKIP) cancelled - brain already have exclamation");
        return false;
    }
    if (exclamation.exMaxCount < 0 && GetExFromStream(exclamation.name))
    {
        _apLog("~BASS~Exclamation (SKIP) cancelled - stream already have exclamation of same group");
        return false;
    }
    if (GetExFromStream(minSimSoundDelay))
    {
        _apLog("~BASS~Exclamation (SKIP) cancelled - stream already have exclamation just started (sim sound)");
        return false;
    }
    return true;
}
