#pragma once
// ws-engine vid: screenshot-queue method surface of vidDRIVER (QueueScreenshot /
// FlushScreenshotQueue / virtual ReadBackBuffer). The former per-consumer partial slice of
// vidDRIVER was consolidated into the canonical full DB-verified layout:
#include "../../vidDRIVER.h"

// Screenshot methods (declared on the canonical vidDRIVER):
//   virtual int ReadBackBuffer(pctPICTURE *pic, txmTEXTURE *backbuf);
//     vtable -- capture the current backbuffer into `pic`, converting/scaling to `backbuf`'s
//     format if needed; returns whether it succeeded. boundary (called from FlushScreenshotQueue).
//   void QueueScreenshot(pctPICTURE *pic);
//     0x82667DB4 -- reversed in vidDRIVER__QueueScreenshot.c. Records `pic` as the pending
//     screenshot target and clears the ready flag (picked up by FlushScreenshotQueue).
//   void FlushScreenshotQueue(txmTEXTURE *backbuf);
//     0x82667DD8 -- reversed in vidDRIVER__FlushScreenshotQueue.c. If a screenshot is queued and
//     not yet ready, attempts to service it via ReadBackBuffer against `backbuf`.
