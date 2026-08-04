#pragma once
/* haloPLAYER_ACTIONS_ENV — the per-tick, both-coop-players envelope of digested player actions
 * handed from the ws-engine input path down into the Blam co-op update
 * (hcex_coop_process_deteministic_data -> haloENGINE_CONTROL::SetDetAction ->
 * hcex_coop_apply_player_actions), and also network-packable in its own right (Pack/Unpack).
 * DB-verified layout (types_members haloPLAYER_ACTIONS_ENV): u0@0x00, u1@0x20, skip_cine@0x40 —
 * 68 bytes (0x44), i.e. skip_cine is followed by 24 bytes of tail padding (net alignment of the
 * containing dsDEQUE<haloPLAYER_ACTIONS_ENV> element).
 *
 * haloPLAYER_ACTION_PACKER (see haloPLAYER_ACTION_PACKER.h) publicly inherits haloPLAYER_ACTION,
 * adding no fields — layout-identical, so existing boundary consumers that reinterpret a u0/u1
 * slot directly as a plain haloPLAYER_ACTION (see hcex_coop_process_deteministic_data) keep
 * working unchanged. */

#include "haloPLAYER_ACTION_PACKER.h"
#include "../ws/ds/dsBIT_STREAM.h"

struct dsDATA; // ws-engine ds — boundary, passed through untouched as a per-stream user context

typedef struct haloPLAYER_ACTIONS_ENV
{
    haloPLAYER_ACTION_PACKER u0;          /* 0x00 local player's digested action */
    haloPLAYER_ACTION_PACKER u1;          /* 0x20 remote/second player's digested action */
    unsigned int             skip_cine;   /* 0x40 set by hcex_coop_process_raw_input when either
                                            * player's raw input requests a cinematic skip */

    // 0x823BB0D4 — default: zero skip_cine (u0/u1 are left default-constructed, which for the
    // haloPLAYER_ACTION base means uninitialised POD fields — matches the binary, which never
    // clears them here).
    haloPLAYER_ACTIONS_ENV();

    // 0x823BC7FC — serialize u0, then u1, then skip_cine (as a raw 32-bit field, byte-reordered
    // when the stream is in reordering mode). Const (QBA).
    void Pack(ds::BIT_STREAM &stream, const dsDATA &userCtx) const;

    // 0x823BC88C — deserialize in the same field order as Pack.
    void Unpack(ds::BIT_STREAM &stream, const dsDATA &userCtx);
} haloPLAYER_ACTIONS_ENV;                 /* 68 bytes (0x44) */
