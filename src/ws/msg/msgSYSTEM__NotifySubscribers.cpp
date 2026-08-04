#include "msgSYSTEM.h"
#include "msgSUBSCRIBER.h"

// msgSYSTEM::NotifySubscribers @ 0x82555ED8
// Broadcast `msg` to every address subscribed to it via the subscription registry.
void msgSYSTEM::NotifySubscribers(int msg, void *pInfo, msgADDR *pSender)
{
    _msgSubscr.SendMsg(msg, pInfo, pSender);
}
