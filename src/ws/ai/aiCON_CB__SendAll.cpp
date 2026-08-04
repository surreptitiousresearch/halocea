#include "../../headers/ws/ai/aiCON_CB.h"

// aiCON_CB::SendAll @ 0x83182530
// Push every AI console sub-list unconditionally (used on (re)connect / page enable). Stops and
// returns false at the first sub-send that fails.
bool aiCON_CB::SendAll()
{
    if (!this->SendWatcheeToConsole(true))  return false;
    if (!this->SendAICountToConsole(true))  return false;
    if (!this->SendListSSLVars(true))       return false;
    if (!this->SendMainList(true))          return false;
    if (!this->SendExtraList(true))         return false;
    if (!this->SendTaskSys(true))           return false;
    if (!this->SendBhvStack(true))          return false;
    return true;
}
