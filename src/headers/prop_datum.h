#pragma once

/* prop_datum — an actor's "prop": its working knowledge of one other unit (enemy/ally/target) it is aware of.
 * 312 bytes (0x138). Layout transcribed verbatim from the database. */

#include <stdint.h>
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
    int16_t        identifier;                       /* 0x00 */
    char           _pad0[2];
    int            owner_actor_index;                /* 0x04 */
    int            next_prop_index;                  /* 0x08 */
    union _135C1444610E6833A296ED2F356DBE75 ___u3;   /* 0x0C — orphan/parent prop index */
    int16_t        type;                             /* 0x10 */
    int16_t        team_index;                       /* 0x12 */
    uint8_t swarm;                           /* 0x14 */
    char           _pad1[3];
    int            unit_index;                       /* 0x18 */
    int            actor_index;                      /* 0x1C */
    float          suicide_radius;                   /* 0x20 */
    int16_t        state;                            /* 0x24 */
    int16_t        timer;                            /* 0x26 */
    int            swarm_unit_selected_time;         /* 0x28 */
    float          awareness;                        /* 0x2C */
    int16_t        perception;                       /* 0x30 */
    int16_t        visibility;                       /* 0x32 */
    int16_t        audibility;                       /* 0x34 */
    int16_t        ineffability;                     /* 0x36 */
    int16_t        line_of_sight;                    /* 0x38 */
    int16_t        orphan_lifespan_ticks;            /* 0x3A */
    int16_t        orphan_inspection_ticks;          /* 0x3C */
    char           _pad2[2];
    real_vector3d  orphan_hint_vector;               /* 0x40 */
    int16_t        ticks_until_orphan;               /* 0x4C */
    uint8_t orphan_corpse_cheated;           /* 0x4E */
    char           _pad3[1];
    float          target_weight;                    /* 0x50 */
    float          look_interest;                    /* 0x54 */
    float          last_idle_look_interest;          /* 0x58 */
    int            last_idle_look_time;              /* 0x5C */
    uint8_t enemy;                           /* 0x60 */
    uint8_t ally;                            /* 0x61 */
    uint8_t ally_status_changed;             /* 0x62 */
    uint8_t in_use;                          /* 0x63 */
    uint8_t refresh_stimuli;                 /* 0x64 */
    char           _pad4[1];
    int16_t        unit_effect;                      /* 0x66 */
    int16_t        unit_effect_decay_ticks;          /* 0x68 */
    int16_t        required_ticks;                   /* 0x6A */
    int16_t        ticks_since_damage;               /* 0x6C */
    char           _pad5[2];
    float          damage_inflicted_on_me;           /* 0x70 */
    uint8_t currently_damaging_me;           /* 0x74 */
    char           _pad6[1];
    int16_t        dead_ticks;                       /* 0x76 */
    int16_t        visible_ticks;                    /* 0x78 */
    char           _pad7[2];
    int            last_perceived_time;              /* 0x7C */
    real_point3d   last_perceived_body_position;     /* 0x80 */
    int            last_visible_time;                /* 0x8C */
    real_point3d   last_visible_head_position;       /* 0x90 */
    int16_t        unreachable_ticks;                /* 0x9C */
    char           _pad8[2];
    int            last_unreachable_time;            /* 0xA0 */
    uint8_t unopposable_enemy;               /* 0xA4 */
    char           _pad9[1];
    int16_t        unopposable_casualties_inflicted; /* 0xA6 */
    int16_t        unopposable_casualty_decay_timer; /* 0xA8 */
    int16_t        unopposable_trigger_hysteresis;   /* 0xAA */
    int16_t        unopposable_trigger_timer;        /* 0xAC */
    int16_t        unopposable_trigger_threshold;    /* 0xAE */
    int16_t        ticks_since_definitely_located;   /* 0xB0 */
    char           _pad10[2];
    int            definite_knowledge_source_actor;  /* 0xB4 */
    uint8_t definitely_located;              /* 0xB8 */
    uint8_t tried_to_uncover;                /* 0xB9 */
    uint8_t tried_to_search;                 /* 0xBA */
    uint8_t abandoned_search;                /* 0xBB */
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
    uint8_t underwater;                       /* 0x118 */
    char           _pad11[3];
    float          distance;                          /* 0x11C */
    char           lighting;                          /* 0x120 */
    char           quantized_distance;                /* 0x121 */
    char           quantized_facing;                  /* 0x122 */
    char           quantized_speed;                   /* 0x123 */
    char           quantized_closing_speed;           /* 0x124 */
    char           child_units_attached;              /* 0x125 */
    uint8_t delay_requirement_decision;       /* 0x126 */
    uint8_t dead;                             /* 0x127 */
    uint8_t really_dead;                      /* 0x128 */
    uint8_t just_killed;                      /* 0x129 */
    uint8_t just_became_visible;              /* 0x12A */
    uint8_t noncombat;                        /* 0x12B */
    uint8_t in_combat;                        /* 0x12C */
    uint8_t fighting;                         /* 0x12D */
    uint8_t player;                           /* 0x12E */
    uint8_t shooting;                         /* 0x12F */
    uint8_t flying;                           /* 0x130 */
    uint8_t active_camouflage;                /* 0x131 */
    uint8_t flashlight;                       /* 0x132 */
    uint8_t ignore;                           /* 0x133 */
    uint8_t preferred_target;                 /* 0x134 */
    uint8_t vehicle_gunner;                   /* 0x135 */
    uint8_t dangerous_vehicle_driver;         /* 0x136 */
    char           _pad12[1];
} prop_datum;
