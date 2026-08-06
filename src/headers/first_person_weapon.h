#pragma once
/* first_person_weapon — viewmodel state for one local player's first-person weapon (first_person_weapons.c),
 * 7840 bytes. Full DB layout: control fields, animation states, sway/turn springs, render pose, and the
 * node-orientation / node-matrix scratch used to pose the viewmodel. */

#include <stdint.h>
#include "animation_state.h"
#include "animation_state_continuous.h"
#include "real_vector2d.h"
#include "real_vector3d.h"
#include "real_point3d.h"
#include "real_euler_angles2d.h"
#include "real_orientation.h"
#include "real_matrix4x3.h"

typedef struct first_person_weapon
{
    uint8_t            visible;                        /* 0x0000 */
    unsigned char              _pad01[3];                      /* 0x0001 */
    int                        unit_index;                     /* 0x0004 */
    int                        weapon_index;                   /* 0x0008 */
    int16_t                    state;                          /* 0x000C */
    int16_t                    ticks_until_pose;               /* 0x000E */
    int16_t                    ticks_idle;                     /* 0x0010 */
    int16_t                    ticks_until_predict;            /* 0x0012 */
    int16_t                    overlay_animation_index;        /* 0x0014 */
    animation_state            state_animation;                /* 0x0016 */
    animation_state            moving_animation;               /* 0x001A */
    int16_t                    _pad1E;                         /* 0x001E */
    animation_state_continuous overcharged_jitter_animation;   /* 0x0020 */
    float                      firing_push_back;               /* 0x0028 */
    float                      firing_push_back_velocity;      /* 0x002C */
    real_vector2d              position;                       /* 0x0030 */
    real_vector2d              position_velocity;              /* 0x0038 */
    real_vector2d              turning;                        /* 0x0040 */
    real_vector2d              turning_velocity;               /* 0x0048 */
    uint8_t            rendered;                       /* 0x0050 */
    unsigned char              _pad51[3];                      /* 0x0051 */
    real_vector3d              render_forward;                 /* 0x0054 */
    real_euler_angles2d        render_facing;                  /* 0x0060 */
    real_euler_angles2d        last_render_facing;             /* 0x0068 */
    real_point3d               render_position;                /* 0x0070 */
    real_point3d               last_render_position;           /* 0x007C */
    int16_t                    interpolation_frame_index;      /* 0x0088 */
    int16_t                    interpolation_frame_count;      /* 0x008A */
    real_orientation           node_orientations[64];          /* 0x008C */
    real_orientation           original_node_orientations[64]; /* 0x088C */
    real_matrix4x3             node_matrices[64];              /* 0x108C */
    uint8_t            weapon_node_remapping_table_valid;/* 0x1D8C */
    uint8_t            _pad1D8D;                       /* 0x1D8D */
    int16_t                    weapon_node_remapping_table[64];/* 0x1D8E */
    uint8_t            hands_node_remapping_table_valid;/* 0x1E0E */
    uint8_t            _pad1E0F;                       /* 0x1E0F */
    int16_t                    hands_node_remapping_table[64]; /* 0x1E10 */
    uint8_t            shotgun_empty;                  /* 0x1E90 */
    uint8_t            _pad1E91;                       /* 0x1E91 */
    int16_t                    shotgun_shells_to_reload;       /* 0x1E92 */
    int16_t                    shotgun_reload_type;            /* 0x1E94 */
    int16_t                    _pad1E96;                       /* 0x1E96 */
    int                        current_sound_index;            /* 0x1E98 */
    int16_t                    current_sound_state;            /* 0x1E9C */
    unsigned char              _pad1E9E[2];                    /* 0x1E9E */
} first_person_weapon;                                         /* 7840 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern first_person_weapon *first_person_weapons;

#ifdef __cplusplus
}
#endif
