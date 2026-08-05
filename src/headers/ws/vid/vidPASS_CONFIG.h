#pragma once
#include <stdint.h>
#include "vidPASS_OBJ.h"
#include "vidPASS_ENV.h"
#include "vidPASS_RENDERSTATE.h"
#include "SDR_FULL_STATES.h"
#include "d3dOBJ_PASS.h"
// ws-engine vid: a full render-pass configuration. DB-verified layout (types_members
// vidPASS_CONFIG) -- size 264. All sub-objects typed per DB (byteshim pass 2026-08-04).

typedef struct vidPASS_CONFIG {
    uint8_t     passId;         // 0x00
    uint8_t     _pad01[3];      // 0x01 alignment
    vidPASS_OBJ         passObj;        // 0x04 (156)
    vidPASS_ENV         passEnv;        // 0xA0 (24)
    uint64_t    passFvf;        // 0xB8
    uint8_t     passOpt;        // 0xC0
    uint8_t     _padC1[3];      // 0xC1
    vidPASS_RENDERSTATE rs;             // 0xC4 (32)
    SDR_FULL_STATES     sdrState;       // 0xE4 (24)
    d3dOBJ_PASS         pass;           // 0xFC (10)
    uint8_t     _pad106[2];     // 0x106 tail padding to DB total
} vidPASS_CONFIG;                       // 264 bytes
