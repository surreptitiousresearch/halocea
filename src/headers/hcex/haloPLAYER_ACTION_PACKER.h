#pragma once
/* haloPLAYER_ACTION_PACKER — network-packable wrapper around a single haloPLAYER_ACTION slot.
 * DB-verified layout (types_members haloPLAYER_ACTION_PACKER): a single unnamed base class,
 * haloPLAYER_ACTION, at offset 0, size 32 (layout-identical to haloPLAYER_ACTION -- adds no
 * fields, no vtable). Modeled as public inheritance so existing boundary consumers that
 * reinterpret a haloPLAYER_ACTION_PACKER as a plain haloPLAYER_ACTION (see
 * hcex_coop_process_deteministic_data) keep working unchanged. */

#include "../haloPLAYER_ACTION.h"
#include "../ws/ds/dsBIT_STREAM.h"

struct dsDATA; // ws-engine ds — boundary, passed through untouched as a per-stream user context

struct haloPLAYER_ACTION_PACKER : haloPLAYER_ACTION {
    // 0x823BC330 — serialize each field of the wrapped haloPLAYER_ACTION into `stream`,
    // byte-reordering each field first when the stream is in reordering mode. Const (QBA).
    void Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const;

    // 0x823BC5A0 — deserialize each field of the wrapped haloPLAYER_ACTION from `stream` in the
    // same field order as Pack, byte-reordering in place when the stream is in reordering mode.
    void Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx);
};
