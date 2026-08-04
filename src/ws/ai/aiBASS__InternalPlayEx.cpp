// aiBASS::InternalPlayEx @ 0x83245EF0
// ?InternalPlayEx@aiBASS@@MAA_NPAVaiBRAIN@@ABVexDATA@1@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSND_BASE.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// Validate `exclamation` for `pBrain` against a chain of gates (valid resolved sound name, non-zero
// max count, within max distance, brain-accepted, survives the stream filter), and on success build
// an exSTREAM_DATA and insert it into the per-brain stream keyed by `pBrain`.
bool aiBASS::InternalPlayEx(aiBRAIN *pBrain, const exDATA &exclamation)
{
    if (!pBrain)
        return false;

    aiSND_BASE *snd = pBrain->mind.snd.pointee;
    dsSTRID sndName = snd->BuildExclamationName(exclamation.name, exclamationPrefix, exclamationPostfix);
    if (!sndName.IsValid())
        return false;

    if (!exclamation.exMaxCount)
    {
        _apLog("~BASS~Exclamation disabled (exMaxCount = 0)");
        return false;
    }

    float dist = pBrain->GetDistToClosestPlayer();
    if (dist > maxDistance)
    {
        _apLog("~BASS~Exclamation cancelled by max dist");
        return false;
    }

    if (!pBrain->mind.snd.pointee->IsExclamationAccepted(exclamation.name))
    {
        _apLog("~BASS~Exclamation cancelled by brain");
        return false;
    }

    if (!FilterExVsStream(pBrain, exclamation, dist))
    {
        _apLog("~BASS~Exclamation cancelled by stream");
        return false;
    }

    exSTREAM_DATA data(&exclamation, &sndName, pBrain, dist);
    exStream.Insert(&pBrain, &data);
    _apLog("~BASS~Exclamation accepted & added to stream");
    return true;
}
