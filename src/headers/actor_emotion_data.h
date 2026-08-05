#pragma once
#include <stdint.h>
/* actor_emotion_data — actor_datum member at 0x350 (848), 104 bytes. Full layout from an authoritative
 * actor_datum structure dump (ground truth; supersedes the prior partial reconstruction).
 *
 * CORRECTION: the prior version of this header named relative offset 0x25 (885) "flee_desire", based on
 * an inline comment in actor_emotion_update.c calling it a "cower/flee desire flag from berserk or the
 * character's panic flag". The dump names it forced_to_charge instead — which fits the evidence better
 * on reflection: berserk actors in this engine charge aggressively, they don't flee, so a flag derived
 * from the berserk state (see actor_berserk.c:47, `if (berserk) actor[885] = 1;`) reads more sensibly
 * as "forced to charge" than "wants to flee". Corrected to match the dump. */

typedef struct actor_emotion_data
{
    float            instantaneous_danger;                       /* 0x00 */
    float            perceived_danger;                            /* 0x04 */
    uint8_t  defensive_crouch;                             /* 0x08 */
    unsigned char    _pad09;                                        /* 0x09 */
    int16_t          defensive_crouch_timer;                        /* 0x0A */
    uint8_t  crouch_blocking_player_line_of_fire;           /* 0x0C */
    uint8_t  crouch_blocking_line_of_fire;                   /* 0x0D */
    uint8_t  crouch_friends_in_line_of_fire;                  /* 0x0E */
    uint8_t  moving_into_player_line_of_fire;                 /* 0x0F */
    int16_t          moving_into_fire_timer;                          /* 0x10 */
    uint8_t  crouch_switching_active;                         /* 0x12 */
    uint8_t  crouch_switching_current_value;                  /* 0x13 */
    int16_t          crouch_switching_change_timer;                   /* 0x14 */
    int16_t          crouch_switching_stay_timer;                     /* 0x16 */
    int16_t          evasion_delay_timer;                             /* 0x18 */
    unsigned char    _pad1A[2];                                       /* 0x1A */
    int              last_defensive_cover_seeking_time;                /* 0x1C */
    int              last_active_cover_seeking_time;                   /* 0x20 */
    uint8_t  currently_defending;                              /* 0x24 */
    uint8_t  forced_to_charge;                                 /* 0x25 */
    uint8_t  ignorant_of_broken_surfaces;                       /* 0x26 */
    uint8_t  sighted_friendly_player;                           /* 0x27 */
    uint8_t  berserk;                                           /* 0x28 */
    uint8_t  played_berserk_sound;                              /* 0x29 */
    unsigned char    _pad2A[2];                                          /* 0x2A */
    int              last_melee_check_time;                              /* 0x2C */
    int              last_melee_attack_time;                             /* 0x30 */
    int              last_vehicle_check_time;                            /* 0x34 */
    int              last_vehicle_charge_time;                           /* 0x38 */
    uint8_t  last_vehicle_exit_forced;                           /* 0x3C */
    unsigned char    _pad3D[3];                                          /* 0x3D */
    int              vehicle_ignore_index;                               /* 0x40 */
    int              vehicle_ignore_time;                                /* 0x44 */
    int              last_flee_failed_time;                              /* 0x48 */
    int              flee_with_friends_disable_time;                    /* 0x4C */
    int              corpse_ignore_time;                                 /* 0x50 */
    int              unopposable_friend_ignore_time;                    /* 0x54 */
    int16_t          unopposable_retreat_timer;                          /* 0x58 */
    unsigned char    _pad5A[2];                                          /* 0x5A */
    int              unopposable_retreat_prop_index;                     /* 0x5C */
    int              unopposable_retreat_start_time;                     /* 0x60 */
    float            original_body_vitality;                            /* 0x64 */
} actor_emotion_data; /* 0x68 = 104 bytes */
