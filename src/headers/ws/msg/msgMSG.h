#pragma once
// ws-engine msg: a single message envelope routed through the message system.
// DB-verified layout (types_members msgMSG):
//   pRecv@0, pSend@4, msg@8, sizeInfo@12, pInfo@16, elapsedTime@20 — size 24.

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem); handled by pointer only

typedef struct msgMSG {
    msgADDR *pRecv;       // 0x00 recipient address
    msgADDR *pSend;       // 0x04 sender address
    int      msg;         // 0x08 message id
    int      sizeInfo;    // 0x0C payload size / info word
    void    *pInfo;       // 0x10 payload pointer
    float    elapsedTime; // 0x14 delivery timestamp/delay

    // 0x82AC3770 — free the payload and reset the envelope to an empty/dead state.
    void DelInfo();
    // 0x82AC36A0 — populate the envelope and deep-copy `sizeInfo` bytes of payload (heap-allocated).
    // Returns 1 on success, 0 if the payload allocation failed.
    int  SetInfo(msgADDR *pRecv, int msg, int sizeInfo, void *pInfo, msgADDR *pSend, float elapsedTime);
} msgMSG;
