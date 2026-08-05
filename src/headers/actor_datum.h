#pragma once
/* actor_datum — one AI actor (0x724 = 1828 bytes). Full top-level and mostly-full sub-struct layout,
 * per an authoritative actor_datum structure dump (ground truth). STALE-NOTE CORRECTED 2026-07-13: the
 * sub-structs a prior version of this comment called "still opaque byte blobs" (actor_path_control_data,
 * actor_look/move/combat_orders, actor_animation_orders, actor_discarded_firing_position) are all fully
 * named structs now; actor_destination_orders' payload union is also fully resolved (DB union
 * $89B12494..., see actor_destination_orders.h) — nothing in this layout remains open.
 *
 * Cross-validation: the danger_zone bounds here (0x280-0x2EC, 640-748) were independently reverse
 * engineered from the memset(actor+640, 0, 0x6C) danger-latch writers (see actor_danger_zone.h) before
 * the ground-truth dump was available, and they match it exactly — as does firing_positions' start
 * (0x3B8/952) against the independently-found current_position_index offset. Two fields the earlier,
 * usage-based reconstruction had named from local context turned out to have different ground-truth
 * names once the dump arrived — see the CORRECTION notes in actor_emotion_data.h (flee_desire ->
 * forced_to_charge) and actor_firing_position_data.h (path_unavailable/arrived ->
 * current_position_found_outside_range/moved_away_from_firing_position).
 *
 * RESOLVED BY THE DUMP: byte 1284 and floats 1304/1308/1312 — previously flagged as reading with
 * genuinely conflicting meanings across different actor_action_handle_* / action_*.c files ("committed-
 * movement flag" in actor_move_update.c vs. "RNG-already-seeded" in actor_action_handle_evasion.c vs.
 * "flee vector"/"idle facing" elsewhere) — turn out to land exactly on named actor_control_data fields:
 * 1284 = control.moving (relative 0x098, a plain boolean), 1304/1308/1312 = control.moving_towards_vector
 * .n[0]/.n[1]/.n[2] (relative 0x0AC). The conflicting interpretations were each a different call site
 * reading the SAME "am I moving" flag / "where am I moving toward" vector for its own local purpose, not
 * actually contradictory once named. Call sites in actor_action_handle_danger_avoidance.c were updated
 * to use these; other files still reading them by raw offset are unchanged (follow-up work).
 *
 * RESOLVED 2026-07-13: byte 352 = input.vehicle_passenger (DB types_members-confirmed at rel 0x40); the
 * write site's "vehicle seat role <= 1" in actor_input_update.c is just how passengership is derived
 * (driver_type 0/1 = not free-moving/turret driver), not a naming conflict. Byte 1196 (a "path
 * destination point", real_point3d) lands inside control.path (relative 0x40) — actor_path_control_data
 * is now fully modeled; rel 0x40 falls in its path_result region, consistent with the usage. */
#include <stdint.h>
#include "actor_meta_data.h"
#include "actor_state_data.h"
#include "actor_input_data.h"
#include "actor_external_orders.h"
#include "actor_situation.h"
#include "actor_target_data.h"
#include "actor_danger_zone.h"
#include "actor_stimulus_data.h"
#include "actor_emotion_data.h"
#include "actor_firing_position_data.h"
#include "actor_orders.h"
#include "actor_control_data.h"
#include "actor_output_data.h"

typedef struct actor_datum
{
    int16_t                     identifier;        /* 0x00 */
    int16_t                     _pad02;            /* 0x02 */
    actor_meta_data              meta;              /* 0x04 — ends exactly at 0x60 */
    actor_state_data             state;             /* 0x60 */
    actor_input_data             input;             /* 0x120 — per-tick sampled unit view (168 bytes) */
    actor_external_orders        external_orders;   /* 0x1C8 — script overrides (36 bytes) */
    actor_situation               situation;         /* 0x1EC (492) */
    unsigned char                 _pad267;           /* 0x267 (615) */
    actor_target_data             target;            /* 0x268 (616) */
    actor_danger_zone             danger_zone;       /* 0x280 (640) — see actor_danger_zone.h */
    actor_stimulus_data           stimuli;           /* 0x2EC (748) */
    actor_emotion_data            emotions;          /* 0x350 (848) — see actor_emotion_data.h */
    actor_firing_position_data    firing_positions;  /* 0x3B8 (952) — see actor_firing_position_data.h */
    actor_orders                  orders;            /* 0x3E8 (1000) */
    actor_control_data            control;           /* 0x46C (1132) */
    actor_output_data             output;            /* 0x6D0 (1744) */
} actor_datum;                                       /* 0x724 = 1828 bytes */
