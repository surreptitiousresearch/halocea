#pragma once
/* firing_position_evaluation_context — the full query/scoring state passed to firing-position evaluators.
 * DB layout (offsets authoritative). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "evaluation_data.h"
#include "firing_position_avoid_point.h"
#include "firing_position_attack_vector.h"

typedef struct firing_position_evaluation_context
{
    unsigned int    allowed_position_mask;                  /* 0x000 */
    int16_t         evaluation_mode;                        /* 0x004 */
    unsigned char _pad0[2]; /* db-verified padding */
    evaluation_data evaluation_data;                        /* 0x008 */
    unsigned char   allow_rejected_positions;               /* 0x014 */
    unsigned char   allow_outside_range;                    /* 0x015 */
    unsigned char _pad1[2]; /* db-verified padding */
    float           maximum_allowable_range;                /* 0x018 */
    float           maximum_search_range;                   /* 0x01C */
    unsigned char   specific_target_enable;                 /* 0x020 */
    unsigned char _pad2[3]; /* db-verified padding */
    real_point3d    specific_target_point;                  /* 0x024 */
    int             specific_target_surface_index;          /* 0x030 */
    int16_t         specific_target_cluster_index;          /* 0x034 */
    unsigned char   attractor_enable;                       /* 0x036 */
    unsigned char _pad3[1]; /* db-verified padding */
    float           attractor_weight;                       /* 0x038 */
    float           attractor_radius;                       /* 0x03C */
    unsigned char   find_path_direction_from_actor;         /* 0x040 */
    unsigned char   use_last_visible_target_position;       /* 0x041 */
    unsigned char   find_path_distance_to_target;           /* 0x042 */
    unsigned char   find_path_direction_from_target;        /* 0x043 */
    unsigned char   flying;                                 /* 0x044 */
    unsigned char   directional_driving;                    /* 0x045 */
    unsigned char   directional_driving_cannot_stop;        /* 0x046 */
    unsigned char _pad4[1]; /* db-verified padding */
    unsigned int    preferred_groups;                       /* 0x048 */
    float           preferred_weight;                       /* 0x04C */
    int             avoid_point_count;                      /* 0x050 */
    firing_position_avoid_point avoid_point[32];            /* 0x054 */
    int16_t         attack_vector_count;                    /* 0x254 */
    int16_t         friend_attack_vector_count;             /* 0x256 */
    int16_t         dangerous_enemy_attack_vector_count;    /* 0x258 */
    unsigned char _pad5[2]; /* db-verified padding */
    firing_position_attack_vector attack_vectors[32];       /* 0x25C */
    unsigned char   has_gun_offset_stand;                   /* 0x5DC */
    unsigned char _pad6[3]; /* db-verified padding */
    real_vector3d   gun_offset_stand;                       /* 0x5E0 */
    unsigned char   has_gun_offset_crouch;                  /* 0x5EC */
    unsigned char _pad7[3]; /* db-verified padding */
    real_vector3d   gun_offset_crouch;                      /* 0x5F0 */
    unsigned char   has_target;                             /* 0x5FC */
    unsigned char _pad8[3]; /* db-verified padding */
    float           target_current_distance;                /* 0x600 */
    real_point3d    target_point;                           /* 0x604 */
    real_point3d    target_head_position;                   /* 0x610 */
    real_point3d    target_line_of_sight_position;          /* 0x61C */
    unsigned char   target_line_of_sight_optional;          /* 0x628 */
    unsigned char _pad9[3]; /* db-verified padding */
    int             target_vehicle_index;                   /* 0x62C */
    int             target_pathfinding_surface_index;       /* 0x630 */
    real_point3d    target_pathfinding_point;               /* 0x634 */
    int16_t         target_cluster_index;                   /* 0x640 */
    unsigned char _pad10[2]; /* db-verified padding */
    int             target_prop_index;                      /* 0x644 */
    unsigned char   target_has_hint_vector;                 /* 0x648 */
    unsigned char _pad11[3]; /* db-verified padding */
    real_vector3d   target_hint_vector;                     /* 0x64C */
    float           target_danger_radius;                   /* 0x658 */
    unsigned char   post_evaluation_bounded;                /* 0x65C */
    unsigned char _pad12[3]; /* db-verified padding */
    float           post_evaluation_bound;                  /* 0x660 */
} firing_position_evaluation_context;                       /* 0x664 */
