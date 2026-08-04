#pragma once
#include "inpDEVICE.h"
// ws-engine inp: records the device+key that triggered a "wait for key press" capture
// (gsINP_SYSTEM::keyPressEvent). DB-verified layout (types_members inpKEY_PRESS_EVENT):
// device@0, keyId@4 — size 8.

typedef struct inpKEY_PRESS_EVENT {
    inpDEVICE *device; // 0x00 device that produced the key press
    int        keyId;  // 0x04 device-relative key/button id
} inpKEY_PRESS_EVENT;
