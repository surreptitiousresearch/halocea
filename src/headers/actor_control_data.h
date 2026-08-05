#pragma once
/* actor_control_data — actor_datum member at 0x46C (1132), 612 bytes. Full layout from an authoritative
 * actor_datum structure dump (ground truth; supersedes the prior opaque placeholder).
 *
 * NOTE: this resolves the movement/path-direction fields (destination point, direction vector, etc.)
 * that a prior cross-file investigation found genuinely conflicting *interpretations* for (see the
 * DEVIATION note in actor_datum.h) — but that investigation was searching for meaning at absolute actor
 * offsets 1196 and 1304/1308/1312, which do not correspond to any single field boundary below (they
 * land mid-struct, inside moving_towards_point/moving_towards_vector's neighborhood at a different
 * relative offset than assumed). Re-deriving those specific call sites against this authoritative layout
 * is follow-up work, not done here.
 *
 * _012FBAEB678DFAA7A6633086E555150E at 0x1A4 (12 bytes) is IDA's auto-generated name for an
 * anonymous union; the DB resolves it to two members overlaid at 0x1A4
 * (current_fire_target_manual_point / current_fire_target_prop_index), reproduced below. */

#include <stdint.h>
#include "actor_path_control_data.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_vector2d.h"
#include "direction_specification.h"

/* DB anonymous union _012FBAEB678DFAA7A6633086E555150E (12 bytes) — actor_control_data member
 * ___u58 at 0x1A4: the current fire target, as a manual aim point or a prop index depending on
 * current_fire_target_type. $-name kept verbatim ($ in identifiers is an MSVC extension),
 * matching the mtlPARAM.h convention. */
typedef union _012FBAEB678DFAA7A6633086E555150E
{
    real_point3d current_fire_target_manual_point;   /* 0x00 */
    int          current_fire_target_prop_index;     /* 0x00 */
} _012FBAEB678DFAA7A6633086E555150E;

typedef struct actor_control_data
{
    actor_path_control_data path;                                      /* 0x000 */
    uint8_t  moving;                                            /* 0x098 */
    uint8_t  moving_forced_by_aiming;                           /* 0x099 */
    uint8_t  movement_complete;                                 /* 0x09A */
    uint8_t  movement_thwarted;                                 /* 0x09B */
    uint8_t  crouching;                                         /* 0x09C */
    unsigned char    _pad09D;                                           /* 0x09D */
    int16_t          moving_facing_direction;                           /* 0x09E */
    real_point3d     moving_towards_point;                              /* 0x0A0 */
    real_vector3d    moving_towards_vector;                             /* 0x0AC */
    real_vector3d    moving_forced_aim_direction;                       /* 0x0B8 */
    uint8_t  jumping_targeted;                                  /* 0x0C4 */
    unsigned char    _pad0C5[3];                                        /* 0x0C5 */
    real_vector2d    jump_alignment_vector;                             /* 0x0C8 */
    float            jump_target_horizontal_vel;                        /* 0x0D0 */
    float            jump_target_vertical_vel;                          /* 0x0D4 */
    int16_t          secondary_look_type;                               /* 0x0D8 */
    int16_t          secondary_look_priority;                           /* 0x0DA */
    int16_t          secondary_look_timer;                              /* 0x0DC */
    unsigned char    _pad0DE[2];                                        /* 0x0DE */
    direction_specification secondary_look_direction;                  /* 0x0E0 */
    uint8_t  idle_major_active;                                 /* 0x0F0 */
    uint8_t  idle_major_is_aiming;                              /* 0x0F1 */
    uint8_t  idle_major_direction_is_interesting;               /* 0x0F2 */
    uint8_t  idle_minor_active;                                 /* 0x0F3 */
    int              idle_facing_timer;                                 /* 0x0F4 */
    int              idle_major_timer;                                  /* 0x0F8 */
    int              idle_minor_timer;                                  /* 0x0FC */
    direction_specification idle_major_direction;                      /* 0x100 */
    direction_specification idle_minor_direction;                      /* 0x110 */
    uint8_t  aiming_away_from_primary;                          /* 0x120 */
    uint8_t  free_facing_vector;                                /* 0x121 */
    uint8_t  optional_facing_vector;                            /* 0x122 */
    uint8_t  desire_stationary_facing;                          /* 0x123 */
    uint8_t  fixed_stationary_facing;                           /* 0x124 */
    uint8_t  face_exactly;                                      /* 0x125 */
    unsigned char    _pad126[2];                                        /* 0x126 */
    float            face_exactly_oversteer_angle;                      /* 0x128 */
    real_vector3d    fixed_stationary_facing_vector;                    /* 0x12C */
    real_vector3d    desired_facing_vector;                             /* 0x138 */
    real_vector3d    desired_aiming_vector;                             /* 0x144 */
    real_vector3d    desired_looking_vector;                            /* 0x150 */
    uint8_t  vector_avoidance_clear_times[8][2];                /* 0x15C */
    int16_t          vector_avoidance_current_direction;                /* 0x16C */
    unsigned char    _pad16E[2];                                        /* 0x16E */
    real_vector3d    vector_avoidance_rotation;                         /* 0x170 */
    float            vector_avoidance_rotation_emergency;               /* 0x17C */
    float            vector_avoidance_rotation_emergency_instantaneous; /* 0x180 */
    int16_t          vector_avoidance_sharp_turn_timer;                 /* 0x184 */
    int16_t          fire_state;                                       /* 0x186 */
    int16_t          fire_state_timer;                                 /* 0x188 */
    int16_t          burst_disable_timer;                               /* 0x18A */
    int16_t          trigger_delay_timer;                               /* 0x18C */
    int16_t          blocked_communication_timer;                       /* 0x18E */
    int16_t          special_fire_delay;                                /* 0x190 */
    int16_t          special_fire_deny_attempts;                        /* 0x192 */
    uint8_t  firing_at_new_target;                              /* 0x194 */
    uint8_t  firing_while_moving;                               /* 0x195 */
    uint8_t  overcharging_weapon;                               /* 0x196 */
    uint8_t  fire_burst_secondary;                              /* 0x197 */
    uint8_t  next_burst_secondary;                              /* 0x198 */
    unsigned char    _pad199[3];                                        /* 0x199 */
    float            weapon_maximum_range;                              /* 0x19C */
    int16_t          current_fire_target_type;                         /* 0x1A0 */
    unsigned char    _pad1A2[2];                                        /* 0x1A2 */
    _012FBAEB678DFAA7A6633086E555150E ___u58;                          /* 0x1A4 (12 bytes) — anonymous union, defined above */
    int              current_fire_target_timer;                        /* 0x1B0 */
    uint8_t  current_fire_target_visible;                      /* 0x1B4 */
    uint8_t  current_fire_target_underwater;                   /* 0x1B5 */
    uint8_t  current_fire_target_superballistic;               /* 0x1B6 */
    uint8_t  current_fire_target_bombardment;                  /* 0x1B7 */
    uint8_t  current_fire_target_outside_active_area;          /* 0x1B8 */
    unsigned char    _pad1B9;                                           /* 0x1B9 */
    int16_t          current_fire_target_line_of_sight;                /* 0x1BA */
    uint8_t  aiming_at_fire_target;                             /* 0x1BC */
    unsigned char    _pad1BD[3];                                        /* 0x1BD */
    real_point3d     current_fire_target_position;                     /* 0x1C0 */
    float            current_fire_target_range;                        /* 0x1CC */
    real_vector3d    current_fire_target_aim_vector;                   /* 0x1D0 */
    float            current_fire_target_distance;                     /* 0x1DC */
    real_point3d     burst_initial_position;                           /* 0x1E0 */
    real_point3d     burst_origin;                                     /* 0x1EC */
    real_vector3d    burst_relative_position;                          /* 0x1F8 */
    real_vector3d    burst_adjustment;                                 /* 0x204 */
    real_point3d     burst_target;                                     /* 0x210 */
    uint8_t  burst_aim_by_vector;                               /* 0x21C */
    unsigned char    _pad21D[3];                                        /* 0x21D */
    real_vector3d    burst_aim_vector;                                 /* 0x220 */
    float            burst_error;                                      /* 0x22C */
    float            burst_damage_modifier;                            /* 0x230 */
    uint8_t  grenade_trying_to_throw;                          /* 0x234 */
    uint8_t  grenade_current_lob;                               /* 0x235 */
    unsigned char    _pad236[2];                                        /* 0x236 */
    int              grenade_last_check_time;                          /* 0x238 */
    real_point3d     grenade_current_target;                           /* 0x23C */
    int              grenade_current_prop_index;                       /* 0x248 */
    int              grenade_current_ignore_object_index;               /* 0x24C */
    real_vector3d    grenade_current_aim_vector;                       /* 0x250 */
    float            grenade_current_aim_speed;                        /* 0x25C */
    uint8_t  idle_vocalization_combat;                          /* 0x260 */
    unsigned char    _pad261;                                           /* 0x261 */
    int16_t          idle_vocalization_timer;                          /* 0x262 */
} actor_control_data; /* 0x264 = 612 bytes */
