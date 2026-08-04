#pragma once
/* actor_danger_zone — an actor's tracked-danger record (108 bytes), embedded in actor_datum at 0x280
 * (640). Confirmed against the authoritative actor_datum member layout (danger_zone spans exactly
 * 0x280-0x2EC, i.e. 640-748). All three "latch a new danger" entry points zero the whole record with
 * memset(actor + 640, 0, 0x6C) before repopulating it: actor_perception_assess_suicide_danger (a
 * fleeing/suicide unit threat), actor_perception_assess_vehicle_danger (an oncoming hostile vehicle),
 * and actor_perception_refresh_test_object (a thrown grenade). The 0x6C memset size is independent
 * ground truth for this struct's length, matching the authoritative layout exactly; every field offset
 * below was cross-referenced against those three writers plus actor_perception_update.c
 * (acknowledge/dive-roll logic), actor_perception_refresh_danger_zone.c (per-tick point/velocity/radius
 * refresh), actor_stimulus_noticed_danger_zone.c, and actor_action_handle_danger_avoidance.c — and the
 * field sizes sum to exactly 108 bytes with no slack.
 *
 * CORRECTION (C:/halo/headers_ref/ authoritative dump, 2026-07-08):
 *   - 0x02: 'source' → 'hostility' (hostility classifier, not a source index)
 *   - 0x04: 'pending_acknowledge_timer' → 'acknowledgement_timer'
 *   - 0x06: 'reacted' → 'currently_perceived'
 *   - 0x07: 'acknowledged' → 'noticed_danger'
 *   - 0x08: 'dive_allowed' → 'allow_dive_evasion'
 *   - 0x09: 'hostility_decided' → 'communicated'
 *   - 0x0A: 'moved_to_owner_team' → 'attached_to_us'
 *   - 0x10: '_pad10[4]' → 'owner_unit_index' (int; was wrongly classified as padding)
 *   - 0x14: 'source_radius' → 'danger_radius'
 *   - 0x18: '_pad18[0..11]' → 'initial_position' (real_point3d; was wrongly classified as padding)
 *   - 0x24: '_pad18[12..23]' → 'initial_velocity' (real_vector3d; was wrongly classified as padding)
 *   - 0x30: 'point' → 'position'
 *   - 0x48: 'predicted_point' → 'predict_danger_position'
 *   - 0x54: 'distance_to_sense_position' → 'current_distance_from_actor'
 *   - 0x58: 'computed_radius' → 'bounding_sphere_radius'
 *   - 0x5C: 'midpoint' → 'bounding_sphere_center'
 *   - 0x68: 'timer' → 'danger_data' union (time_until_explosion / time_until_death per type)
 * CORRECTION (headers_ref authoritative dump, 2026-07-11):
 *   - 0x00: 'type' → 'danger_type' (matches actor_danger_zone::danger_type)
 * NOTE: the DB models 0x68 as an anonymous union _F6CEC85A0D4B3331078E024E2EDFE4AE (member ___u19)
 *   of three single-field sub-structs — actor_danger_projectile{time_until_explosion},
 *   actor_danger_vehicle{pad}, actor_danger_suicide{time_until_death} — reproduced verbatim below
 *   ($ in identifiers is an MSVC extension, matching the mtlPARAM.h convention). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "actor_danger_zone_type.h"
#include "actor_danger_projectile.h"
#include "actor_danger_vehicle.h"
#include "actor_danger_suicide.h"

/* DB anonymous union _F6CEC85A0D4B3331078E024E2EDFE4AE (2 bytes) — actor_danger_zone member
 * ___u19 at 0x68: per-danger-type countdown, selected by danger_type. */
typedef union _F6CEC85A0D4B3331078E024E2EDFE4AE
{
    actor_danger_projectile projectile;   /* 0x00 — danger_type == projectile (2) */
    actor_danger_vehicle    vehicle;      /* 0x00 — danger_type == vehicle (3) or none */
    actor_danger_suicide    suicide;      /* 0x00 — danger_type == suicide (1) */
} _F6CEC85A0D4B3331078E024E2EDFE4AE;

typedef struct actor_danger_zone
{
    __int16          danger_type;                 /* 0x00 — actor_danger_zone_type: 0 none, 1 suicide, 2 projectile, 3 vehicle */
    __int16          hostility;                  /* 0x02 — hostility classifier: 0 unclassified/enemy,
                                                     *        1 friendly non-target thrower, 2 thrown by current target */
    __int16          acknowledgement_timer;      /* 0x04 — counts toward 0; >0 gates the acknowledge/dive rolls */
    unsigned __int8  currently_perceived;        /* 0x06 — actor has perceived/reacted to this danger */
    unsigned __int8  noticed_danger;             /* 0x07 — danger noticed (latched; gates dive + stimulus firing) */
    unsigned __int8  allow_dive_evasion;         /* 0x08 — permits actor_action_try_to_dive this tick */
    unsigned __int8  communicated;               /* 0x09 — one-shot latch: the hostility/communication decision ran */
    unsigned __int8  attached_to_us;             /* 0x0A — danger reassigned to the actor's own team; suppresses avoidance */
    unsigned char    _pad0B;                     /* 0x0B */
    int              object_index;               /* 0x0C — the tracked danger object's datum index */
    int              owner_unit_index;           /* 0x10 — CORRECTION: was _pad10[4], now identified as owner_unit_index */
    float            danger_radius;              /* 0x14 — the danger source's own radius (suicide/vehicle/grenade tag) */
    real_point3d     initial_position;           /* 0x18 — CORRECTION: was _pad18[0..11], now identified as initial_position */
    real_vector3d    initial_velocity;           /* 0x24 — CORRECTION: was _pad18[12..23], now identified as initial_velocity */
    real_point3d     position;                   /* 0x30 — current danger position */
    real_vector3d    velocity;                   /* 0x3C — danger's velocity */
    real_point3d     predict_danger_position;    /* 0x48 — point + velocity * 45 ticks */
    float            current_distance_from_actor;/* 0x54 */
    float            bounding_sphere_radius;     /* 0x58 — half-extent(point..predict_danger_position) + danger_radius */
    real_point3d     bounding_sphere_center;     /* 0x5C — midpoint(point, predict_danger_position) */
    _F6CEC85A0D4B3331078E024E2EDFE4AE ___u19;    /* 0x68 — anonymous union, defined above */
    unsigned char    _pad6A[2];                  /* 0x6A */
} actor_danger_zone;                             /* 0x6C = 108 bytes */
