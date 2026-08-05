#pragma once
/* _object_datum — the 496-byte runtime payload of a live object instance, embedded by-value inside
 * object_datum (which prefixes it with a 4-byte definition index). This is the level-specific field
 * block; the composite lives in object_datum.h. Full layout from the database (types_members). */

#include <stdint.h>
#include "networked_datum_role.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "object_animation_state.h"
#include "location.h"
#include "real_rgb_color.h"
#include "object_header_block_reference.h"

typedef struct _object_datum
{
    NetworkedDatumRole datum_role;                              /* 0x00 */
    unsigned char    network_at_rest;                          /* 0x04 */
    unsigned char    was_network_at_rest;                      /* 0x05 */
    char             _pad06[2];                                /* 0x06 */
    int              actor_variant_definition;                 /* 0x08 */
    unsigned int     flags;                                    /* 0x0C */
    int              magic_number;                             /* 0x10 */
    unsigned char    is_server_position_valid;                 /* 0x14 */
    char             _pad15[3];                                /* 0x15 */
    real_point3d     last_server_position;                     /* 0x18 */
    unsigned char    is_server_orientation_valid;              /* 0x24 */
    char             _pad25[3];                                /* 0x25 */
    real_vector3d    last_server_forward;                      /* 0x28 */
    real_vector3d    last_server_up;                           /* 0x34 */
    unsigned char    is_server_translational_velocity_valid;   /* 0x40 */
    char             _pad41[3];                                /* 0x41 */
    real_vector3d    last_server_translational_velocity;       /* 0x44 */
    unsigned char    is_update_timestamp_valid;                /* 0x50 */
    char             _pad51[3];                                /* 0x51 */
    unsigned int     last_update_timestamp;                    /* 0x54 */
    real_point3d     position;                                 /* 0x58 */
    real_vector3d    translational_velocity;                   /* 0x64 */
    real_vector3d    forward;                                  /* 0x70 */
    real_vector3d    up;                                       /* 0x7C */
    real_vector3d    angular_velocity;                         /* 0x88 */
    location         location;                                 /* 0x94 */
    real_point3d     bounding_sphere_center;                   /* 0x9C */
    float            bounding_sphere_radius;                   /* 0xA8 */
    float            scale;                                    /* 0xAC */
    int16_t          type;                                     /* 0xB0 — object type (0 = biped) */
    uint16_t render_flags;                             /* 0xB2 */
    int16_t          owner_team_index;                         /* 0xB4 */
    int16_t          name_index;                               /* 0xB6 */
    int16_t          idle_ticks;                               /* 0xB8 */
    int16_t          variant_number;                           /* 0xBA */
    int              owner_player_index;                       /* 0xBC */
    int              owner_object_index;                       /* 0xC0 */
    int              owner_object_definition_index;            /* 0xC4 */
    object_animation_state animation;                          /* 0xC8 */
    float            maximum_body_vitality;                    /* 0xD4 */
    float            maximum_shield_vitality;                  /* 0xD8 */
    float            body_vitality;                            /* 0xDC */
    float            shield_vitality;                          /* 0xE0 */
    float            current_shield_damage;                    /* 0xE4 */
    float            current_body_damage;                      /* 0xE8 */
    int              umbrella_shield_object_index;             /* 0xEC */
    float            recent_shield_damage;                     /* 0xF0 */
    float            recent_body_damage;                       /* 0xF4 */
    int              shield_damage_decay_timer;                /* 0xF8 */
    int              body_damage_decay_timer;                  /* 0xFC */
    int16_t          shield_stun_ticks;                        /* 0x100 */
    uint16_t damage_flags;                             /* 0x102 */
    int              scenery_idx;                              /* 0x104 */
    int              first_cluster_reference_index;            /* 0x108 */
    int              next_garbage_object_index;                /* 0x10C */
    int              next_object_index;                        /* 0x110 */
    int              first_child_object_index;                 /* 0x114 */
    int              parent_object_index;                      /* 0x118 — -1 when not parented */
    char             parent_node_index;                        /* 0x11C */
    unsigned char    pad[1];                                   /* 0x11D */
    unsigned char    force_shield_update;                      /* 0x11E */
    unsigned char    functions_active_flags;                   /* 0x11F */
    float            incoming_function_values[4];              /* 0x120 */
    float            outgoing_function_values[4];              /* 0x130 */
    char             attachment_types[8];                      /* 0x140 */
    int              attachment_indices[8];                    /* 0x148 */
    int              first_widget_index;                       /* 0x168 */
    int              cached_render_state_index;                /* 0x16C */
    uint16_t regions_destroyed_flags;                  /* 0x170 */
    int16_t          forced_shader_permutation_index;          /* 0x172 */
    unsigned char    region_damage[8];                         /* 0x174 */
    unsigned char    region_permutations[8];                   /* 0x17C */
    real_rgb_color   base_change_colors[4];                    /* 0x184 */
    real_rgb_color   outgoing_change_colors[4];                /* 0x1B4 */
    object_header_block_reference original_node_orientations;  /* 0x1E4 */
    object_header_block_reference node_orientations;           /* 0x1E8 */
    object_header_block_reference node_matrices;               /* 0x1EC */
} _object_datum;                                              /* 496 bytes */
