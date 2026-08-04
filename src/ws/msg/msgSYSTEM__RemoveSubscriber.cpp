#include "msgSYSTEM.h"
#include "msgSUBSCRIBER.h"

// msgSYSTEM::RemoveSubscriber @ 0x82555E78
// Unsubscribe `pSubscr`: from a single message id when msg > 0, or from all message ids otherwise.
void msgSYSTEM::RemoveSubscriber(msgADDR *pSubscr, int msg)
{
    if (msg > 0)
        _msgSubscr.UnRegisterPair(pSubscr, msg);
    else
        _msgSubscr.RemoveSubscriber(pSubscr);
}
