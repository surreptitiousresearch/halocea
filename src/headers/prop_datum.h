#pragma once

/* prop_datum — an actor's "prop": its working knowledge of one other unit (enemy/ally/target) it is aware of.
 * 312 bytes (0x138). Layout transcribed verbatim from the database. */

#include "real_vector3d.h"
#include "real_point3d.h"
#include "location.h"

/* DB anonymous union _135C1444610E6833A296ED2F356DBE75 (4 bytes) — prop_datum member ___u3
 * ($ in identifiers is an MSVC extension). */
typedef union _135C1444610E6833A296ED2F356DBE75
{
    int orphan_prop_index; /* 0x00 */
    int parent_prop_index; /* 0x00 */
} _135C1444610E6833A296ED2F356DBE75;

typedef struct prop_datum
{
    __int16        identifier;                       /* 0x00 */
    char           _pad0[2];
    int            owner_actor_index;                /* 0x04 */
    int            next_prop_index;                  /* 0x08 */
    union _135C1444610E6833A296ED2F356DBE75 ___u3;   /* 0x0C — orphan/parent prop index */
    __int16        type;                             /* 0x10 */
    __int16        team_index;                       /* 0x12 */
    unsigned __int8 swarm;                           /* 0x14 */
    char           _pad1[3];
    int            unit_index;                       /* 0x18 */
    int            actor_index;                      /* 0x1C */
    float          suicide_radius;                   /* 0x20 */
    __int16        state;                            /* 0x24 */
    __int16        timer;                            /* 0x26 */
    int            swarm_unit_selected_time;         /* 0x28 */
    float          awareness;                        /* 0x2C */
    __int16        perception;                       /* 0x30 */
    __int16        visibility;                       /* 0x32 */
    __int16        audibility;                       /* 0x34 */
    __int16        ineffability;                     /* 0x36 */
    __int16        line_of_sight;                    /* 0x38 */
    __int16        orphan_lifespan_ticks;            /* 0x3A */
    __int16        orphan_inspection_ticks;          /* 0x3C */
    char           _pad2[2];
    real_vector3d  orphan_hint_vector;               /* 0x40 */
    __int16        ticks_until_orphan;               /* 0x4C */
    unsigned __int8 orphan_corpse_cheated;           /* 0x4E */
    char           _pad3[1];
    float          target_weight;                    /* 0x50 */
    float          look_interest;                    /* 0x54 */
    float          last_idle_look_interest;          /* 0x58 */
    int            last_idle_look_time;              /* 0x5C */
    unsigned __int8 enemy;                           /* 0x60 */
    unsigned __int8 ally;                            /* 0x61 */
    unsigned __int8 ally_status_changed;             /* 0x62 */
    unsigned __int8 in_use;                          /* 0x63 */
    unsigned __int8 refresh_stimuli;                 /* 0x64 */
    char           _pad4[1];
    __int16        unit_effect;                      /* 0x66 */
    __int16        unit_effect_decay_ticks;          /* 0x68 */
    __int16        required_ticks;                   /* 0x6A */
    __int16        ticks_since_damage;               /* 0x6C */
    char           _pad5[2];
    float          damage_inflicted_on_me;           /* 0x70 */
    unsigned __int8 currently_damaging_me;           /* 0x74 */
    char           _pad6[1];
    __int16        dead_ticks;                       /* 0x76 */
    __int16        visible_ticks;                    /* 0x78 */
    char           _pad7[2];
    int            last_perceived_time;              /* 0x7C */
    real_point3d   last_perceived_body_position;     /* 0x80 */
    int            last_visible_time;                /* 0x8C */
    real_point3d   last_visible_head_position;       /* 0x90 */
    __int16        unreachable_ticks;                /* 0x9C */
    char           _pad8[2];
    int            last_unreachable_time;            /* 0xA0 */
    unsigned __int8 unopposable_enemy;               /* 0xA4 */
    char           _pad9[1];
    __int16        unopposable_casualties_inflicted; /* 0xA6 */
    __int16        unopposable_casualty_decay_timer; /* 0xA8 */
    __int16        unopposable_trigger_hysteresis;   /* 0xAA */
    __int16        unopposable_trigger_timer;        /* 0xAC */
    __int16        unopposable_trigger_threshold;    /* 0xAE */
    __int16        ticks_since_definitely_located;   /* 0xB0 */
    char           _pad10[2];
    int            definite_knowledge_source_actor;  /* 0xB4 */
    unsigned __int8 definitely_located;              /* 0xB8 */
    unsigned __int8 tried_to_uncover;                /* 0xB9 */
    unsigned __int8 tried_to_search;                 /* 0xBA */
    unsigned __int8 abandoned_search;                /* 0xBB */
    real_point3d   body_position;                    /* 0xBC */
    real_point3d   center_of_mass;                   /* 0xC8 */
    real_vector3d  velocity;                         /* 0xD4 */
    real_vector3d  actor_to_prop;                    /* 0xE0 */
    int            pathfinding_surface_index;        /* 0xEC */
    real_point3d   pathfinding_point;                /* 0xF0 */
    location       body_location;                    /* 0xFC */
    real_point3d   head_position;                    /* 0x104 */
    int            vehicle_index;                     /* 0x110 */
    int            attached_to_unit_index;            /* 0x114 */
    unsigned __int8 underwater;                       /* 0x118 */
    char           _pad11[3];
    float          distance;                          /* 0x11C */
    char           lighting;                          /* 0x120 */
    char           quantized_distance;                /* 0x121 */
    char           quantized_facing;                  /* 0x122 */
    char           quantized_speed;                   /* 0x123 */
    char           quantized_closing_speed;           /* 0x124 */
    char           child_units_attached;              /* 0x125 */
    unsigned __int8 delay_requirement_decision;       /* 0x126 */
    unsigned __int8 dead;                             /* 0x127 */
    unsigned __int8 really_dead;                      /* 0x128 */
    unsigned __int8 just_killed;                      /* 0x129 */
    unsigned __int8 just_became_visible;              /* 0x12A */
    unsigned __int8 noncombat;                        /* 0x12B */
    unsigned __int8 in_combat;                        /* 0x12C */
    unsigned __int8 fighting;                         /* 0x12D */
    unsigned __int8 player;                           /* 0x12E */
    unsigned __int8 shooting;                         /* 0x12F */
    unsigned __int8 flying;                           /* 0x130 */
    unsigned __int8 active_camouflage;                /* 0x131 */
    unsigned __int8 flashlight;                       /* 0x132 */
    unsigned __int8 ignore;                           /* 0x133 */
    unsigned __int8 preferred_target;                 /* 0x134 */
    unsigned __int8 vehicle_gunner;                   /* 0x135 */
    unsigned __int8 dangerous_vehicle_driver;         /* 0x136 */
    char           _pad12[1];
} prop_datum;
