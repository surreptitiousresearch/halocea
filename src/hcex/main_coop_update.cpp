/* main_coop_update @0x823C0DB8 — one co-op networking tick. Returns true when a deterministic
 * action set was produced/consumed this call (so the caller advances the game a tick).
 *
 * With no active sync session it just resets the co-op message handler's carried state and
 * reports "ready". Otherwise, once the relay is up and no queued ticks remain to apply, it
 * splits on which peer we are (net::RELAY::GetLocalClientId):
 *   - Local authority (id != 0): sample our raw input, tag it (msgId/tick/crc), relay it, and if
 *     the remote peer already queued a deterministic action set, dequeue and apply the oldest.
 *   - Remote follower (id == 0): give up if we've fallen >=10 ticks behind the last raw input;
 *     otherwise feed the freshly sampled + last-received raw inputs through the Blam action
 *     digester, relay the resulting deterministic set, CRC-check, and apply it. */

#include "../headers/hcex/coop_input_boundary.h"

extern "C" void *memcpy(void *dst, const void *src, unsigned int n);
extern "C" void *memset(void *dst, int c, unsigned int n);

bool main_coop_update()
{
    if (!pSyncSession)
    {
        // DEVIATION: the disassembly open-codes this as a 6-int pointer walk starting just past
        // detInput.length — that span is exactly the 24-byte lastRawInput record. Zero it, then
        // mark "no packet received yet".
        memset(&coopMsgHandler.lastRawInput, 0, sizeof(coopMsgHandler.lastRawInput));
        coopMsgHandler.lastRawInput.msgId = -1;
        coopMsgHandler.lastRawInput.crc = -1;
        return true;
    }

    if (!pSyncSession->m_pRelay)
        return false;

    if (hcex_coop_nticks_to_apply > 0)
    {
        ++hcex_num_update;
        return true;
    }

    net::RELAY   *relay   = pSyncSession->m_pRelay;
    net::CLIENT_ID localId = relay->GetLocalClientId();
    unsigned char didUpdate = 0;

    if (localId)
    {
        // --- local authority: publish our raw input, apply any queued remote deterministic set --
        dsDATA data;
        data.type = nullptr;
        haloRAW_INPUT_X360_ENV *env = dsDATA_Construct<haloRAW_INPUT_X360_ENV>(&data);

        haloEngineCtrl->GetRawInput(localId, env);
        env->msgId = msgId++;
        env->tick  = hcex_data_applied_num;
        env->crc   = game_state_calculate_crc();

        pSyncSession->GetMessageDispatcher()->SendMessage((net::CLIENT_MASK)1, mp::MSG_RAW_INPUT, &data);

        if (coopMsgHandler.detInput.length)
        {
            haloPLAYER_ACTIONS_ENV actions;
            memcpy(&actions, coopMsgHandler.detInput.Front(), sizeof(actions));
            coopMsgHandler.detInput.ResizeFront(coopMsgHandler.detInput.length - 1);
            hcex_collect_crc();
            hcex_coop_process_deteministic_data(&actions);
            didUpdate = 1;
        }

        if (data.type)
            data.type->Destroy((unsigned int *)&data);
    }
    else
    {
        // --- remote follower: digest raw input into a deterministic set, relay + apply it -------
        if (hcex_data_applied_num - coopMsgHandler.lastRawInput.tick >= 10)
            return false;

        haloRAW_INPUT_X360 rawInput;
        haloEngineCtrl->GetRawInput(0, &rawInput);
        haloEngineCtrl->SetRawInput(rawInput, coopMsgHandler.lastRawInput);

        haloPLAYER_ACTION detAction;
        if (haloEngineCtrl->GetDetAction(&detAction))
        {
            dsDATA data;
            data.type = nullptr;
            haloPLAYER_ACTIONS_ENV *env = dsDATA_Construct<haloPLAYER_ACTIONS_ENV>(&data);

            // DEVIATION: the compiler open-coded two overlapping field copies of the retrieved
            // action into the envelope (a 32-byte copy into u0, then an 8-dword copy from
            // desired_zoom_level onward). Reproduced as the same two pointer walks the disasm does;
            // the second reads past detAction's 32 bytes exactly as the original binary does.
            {
                int       *dst = (int *)&env->u0;
                const int *src = (const int *)&detAction;
                for (int n = 0; n < 8; ++n)
                    dst[n] = src[n];

                short       *dstZoom = &env->u0.desired_zoom_level;
                const short *srcZoom = &detAction.desired_zoom_level;
                for (int n = 0; n < 8; ++n)
                {
                    dstZoom += 2;
                    srcZoom += 2;
                    *(int *)dstZoom = *(const int *)srcZoom;
                }
            }

            hcex_coop_process_raw_input(&rawInput, &coopMsgHandler.lastRawInput, env);
            pSyncSession->GetMessageDispatcher()->SendMessage((net::CLIENT_MASK)2, mp::MSG_DET_INPUT, &data);
            hcex_check_crc();
            hcex_coop_process_deteministic_data(env);
            didUpdate = 1;

            if (data.type)
                data.type->Destroy((unsigned int *)&data);
        }
    }

    if (didUpdate)
        ++hcex_num_update;
    return didUpdate != 0;
}
