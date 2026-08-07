#pragma once
/* haloRAW_INPUT_X360_ENV — network-packable envelope around one frame's raw Xbox 360 gamepad
 * state (haloRAW_INPUT_X360), tagged with a message id / tick / crc for the coop input-replication
 * path. DB-verified layout (types_members haloRAW_INPUT_X360_ENV): haloRAW_INPUT_X360 base@0x00
 * (12 bytes), msgId@0x0C, tick@0x10, crc@0x14 — 24 bytes. */

#include "haloRAW_INPUT_X360.h"
#include "../ws/ds/dsBIT_STREAM.h"

struct dsDATA; // ws-engine ds — boundary, passed through untouched as a per-stream user context

typedef struct haloRAW_INPUT_X360_ENV : haloRAW_INPUT_X360
{
    int msgId;   /* 0x0C monotonically increasing send id */
    int tick;    /* 0x10 game tick this input applies to */
    int crc;     /* 0x14 sender's game-state CRC at that tick */

    // 0x823BD180 — serialize msgId, tick, crc, then the base haloRAW_INPUT_X360 fields (buttons,
    // sThumbLX, sThumbLY, sThumbRX, sThumbRY), byte-reordering each field first when the stream is
    // in reordering mode. Const (QBA).
    void Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const;

    // 0x823BD3A8 — deserialize in the same field order as Pack.
    void Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx);
} haloRAW_INPUT_X360_ENV;   /* 24 bytes */
