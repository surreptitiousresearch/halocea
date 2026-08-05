#pragma once
#include <stdint.h>
/* encounter_datum — one AI encounter (108 bytes). Fully named per an authoritative structure dump
 * (C:\halo\headers_ref\encounter_datum.h, 2026-07-09) — resolves the pursuit chain (first_pursuit_index),
 * respawn flag/timer, grenade-rationing stamp (last_grenade_throw_time), encounter-level playfighting,
 * and follow-target fields that were previously opaque `_head1`/`_opaque` blobs. The follow-target
 * index at 0x64 is an anonymous union (DB _7E1B7727F59FD28AE082523475BFB944): a unit index or an
 * ai index, interpreted per follow_target_type. $-name kept verbatim ($ in identifiers is an MSVC
 * extension), matching the mtlPARAM.h convention. */

/* DB union _7E1B7727F59FD28AE082523475BFB944 — the follow-target index overlay (4 bytes). */
typedef union _7E1B7727F59FD28AE082523475BFB944
{
    int follow_target_unit_index;   /* follow_target_type interprets as a unit index */
    int follow_target_ai_index;     /* ... or an ai index */
} _7E1B7727F59FD28AE082523475BFB944;

typedef struct encounter_datum
{
    int16_t         identifier;                  /* 0x00 */
    int16_t         team_index;                  /* 0x02 */
    int16_t         squad_base;                  /* 0x04 */
    int16_t         squad_count;                 /* 0x06 */
    int16_t         platoon_base;                /* 0x08 */
    int16_t         platoon_count;               /* 0x0A */
    uint8_t force_active;                /* 0x0C */
    uint8_t active;                       /* 0x0D */
    int16_t         remain_active_timer;          /* 0x0E */
    int             last_active_time;             /* 0x10 */
    int             first_actor_index;            /* 0x14 */
    int16_t         original_count;               /* 0x18 */
    int16_t         prebattle_living_count;       /* 0x1A */
    int16_t         unique_leader_count;          /* 0x1C */
    uint8_t is_prevehicle_encounter;      /* 0x1E */
    uint8_t _pad1F;                       /* 0x1F */
    int16_t         link_encounter_count;         /* 0x20 */
    int16_t         link_encounter_indices[3];    /* 0x22 */
    uint8_t status_dirty;                /* 0x28 — perception status needs recompute */
    uint8_t _pad29;                       /* 0x29 */
    int16_t         current_count;                /* 0x2A */
    int16_t         current_swarm_count;          /* 0x2C */
    int16_t         current_in_combat_count;      /* 0x2E */
    int16_t         current_fighting_count;       /* 0x30 */
    uint8_t _pad32;                       /* 0x32 */
    uint8_t _pad33;                       /* 0x33 */
    float           current_strength_fraction;    /* 0x34 */
    int             first_pursuit_index;          /* 0x38 */
    uint8_t respawn_enabled;              /* 0x3C */
    uint8_t _pad3D;                       /* 0x3D */
    int16_t         respawn_delay_ticks;          /* 0x3E */
    uint8_t blind;                       /* 0x40 */
    uint8_t deaf;                        /* 0x41 */
    uint8_t stand_down;                  /* 0x42 */
    uint8_t enemy_target;                /* 0x43 */
    uint8_t enemy_alive;                 /* 0x44 */
    uint8_t enemy_visible;               /* 0x45 */
    uint8_t enemy_traitor;               /* 0x46 */
    uint8_t post_combat;                 /* 0x47 */
    uint8_t post_combat_delay;           /* 0x48 */
    uint8_t _pad49;                      /* 0x49 */
    int16_t         post_combat_delay_timer;     /* 0x4A */
    int16_t         enemies_defeated;            /* 0x4C */
    int16_t         _pad4E;                      /* 0x4E */
    int             enemy_visible_timer;         /* 0x50 */
    int             enemy_alive_timer;           /* 0x54 */
    int             corpse_ignore_time;          /* 0x58 */
    int             last_grenade_throw_time;     /* 0x5C */
    uint8_t playfighting;                /* 0x60 */
    uint8_t _pad61;                      /* 0x61 */
    int16_t         follow_target_type;          /* 0x62 — enum ai_follow_target_type */
    union _7E1B7727F59FD28AE082523475BFB944 ___u43; /* 0x64 — interpretation depends on follow_target_type */
    float           follow_target_distance;      /* 0x68 */
} encounter_datum;                               /* 108 bytes */
