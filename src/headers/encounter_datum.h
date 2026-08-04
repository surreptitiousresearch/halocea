#pragma once
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
    __int16         identifier;                  /* 0x00 */
    __int16         team_index;                  /* 0x02 */
    __int16         squad_base;                  /* 0x04 */
    __int16         squad_count;                 /* 0x06 */
    __int16         platoon_base;                /* 0x08 */
    __int16         platoon_count;               /* 0x0A */
    unsigned __int8 force_active;                /* 0x0C */
    unsigned __int8 active;                       /* 0x0D */
    __int16         remain_active_timer;          /* 0x0E */
    int             last_active_time;             /* 0x10 */
    int             first_actor_index;            /* 0x14 */
    __int16         original_count;               /* 0x18 */
    __int16         prebattle_living_count;       /* 0x1A */
    __int16         unique_leader_count;          /* 0x1C */
    unsigned __int8 is_prevehicle_encounter;      /* 0x1E */
    unsigned __int8 _pad1F;                       /* 0x1F */
    __int16         link_encounter_count;         /* 0x20 */
    __int16         link_encounter_indices[3];    /* 0x22 */
    unsigned __int8 status_dirty;                /* 0x28 — perception status needs recompute */
    unsigned __int8 _pad29;                       /* 0x29 */
    __int16         current_count;                /* 0x2A */
    __int16         current_swarm_count;          /* 0x2C */
    __int16         current_in_combat_count;      /* 0x2E */
    __int16         current_fighting_count;       /* 0x30 */
    unsigned __int8 _pad32;                       /* 0x32 */
    unsigned __int8 _pad33;                       /* 0x33 */
    float           current_strength_fraction;    /* 0x34 */
    int             first_pursuit_index;          /* 0x38 */
    unsigned __int8 respawn_enabled;              /* 0x3C */
    unsigned __int8 _pad3D;                       /* 0x3D */
    __int16         respawn_delay_ticks;          /* 0x3E */
    unsigned __int8 blind;                       /* 0x40 */
    unsigned __int8 deaf;                        /* 0x41 */
    unsigned __int8 stand_down;                  /* 0x42 */
    unsigned __int8 enemy_target;                /* 0x43 */
    unsigned __int8 enemy_alive;                 /* 0x44 */
    unsigned __int8 enemy_visible;               /* 0x45 */
    unsigned __int8 enemy_traitor;               /* 0x46 */
    unsigned __int8 post_combat;                 /* 0x47 */
    unsigned __int8 post_combat_delay;           /* 0x48 */
    unsigned __int8 _pad49;                      /* 0x49 */
    __int16         post_combat_delay_timer;     /* 0x4A */
    __int16         enemies_defeated;            /* 0x4C */
    __int16         _pad4E;                      /* 0x4E */
    int             enemy_visible_timer;         /* 0x50 */
    int             enemy_alive_timer;           /* 0x54 */
    int             corpse_ignore_time;          /* 0x58 */
    int             last_grenade_throw_time;     /* 0x5C */
    unsigned __int8 playfighting;                /* 0x60 */
    unsigned __int8 _pad61;                      /* 0x61 */
    __int16         follow_target_type;          /* 0x62 — enum ai_follow_target_type */
    union _7E1B7727F59FD28AE082523475BFB944 ___u43; /* 0x64 — interpretation depends on follow_target_type */
    float           follow_target_distance;      /* 0x68 */
} encounter_datum;                               /* 108 bytes */
