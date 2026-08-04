/* coop_input_initialize @0x823BB198 — enter co-op input mode for `pSession`: remember it as the
 * active sync session and subscribe the co-op message handler to both relay input channels
 * (raw pad input and digested deterministic input), for messages from any client (-1 filter). */

#include "../headers/hcex/coop_input_boundary.h"

void coop_input_initialize(SESSION_BASE *pSession)
{
    pSyncSession = pSession;

    mp::MESSAGE_DISPATCHER *dispatcher = pSession->GetMessageDispatcher();
    dispatcher->SubscribeMessage(mp::MSG_RAW_INPUT, &coopMsgHandler, -1);

    dispatcher = pSyncSession->GetMessageDispatcher();
    dispatcher->SubscribeMessage(mp::MSG_DET_INPUT, &coopMsgHandler, -1);
}
