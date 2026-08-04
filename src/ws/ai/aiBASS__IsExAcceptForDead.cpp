// aiBASS::IsExAcceptForDead @ 0x83240460
// ?IsExAcceptForDead@aiBASS@@UAA_NABVdsSTRID@@@Z
#include "../../headers/ws/ai/aiBASS.h"

// True when exclamation `exID` is a configured, usable category (valid name, non-negative priority,
// a real overlap policy) that is allowed to play for a dead speaker.
// NOTE: the original copies the whole exDATA out of FindExclamation before testing — reproduced.
bool aiBASS::IsExAcceptForDead(const dsSTRID &exID)
{
    exDATA data = FindExclamation(exID);
    if (data.priority < 0.0f || data.overlap == exOVERLAP_INVALID || !data.name.IsValid())
        return false;
    return data.isAcceptForDead;
}
