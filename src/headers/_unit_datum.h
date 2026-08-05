#pragma once
/* _unit_datum — the 728-byte unit-specific runtime payload embedded by-value inside unit_datum
 * (which prefixes it with a 4-byte definition index and the 496-byte _object_datum object block).
 * This is the level-specific unit field block; the composite lives in unit_datum.h.
 * Full layout DB-verified from the database (types_members). */

#include <stdint.h>
#include "real_vector3d.h"
#include "real_point3d.h"
#include "unit_animation.h"
#include "unit_speech.h"
#include "unit_attacker.h"
#include "unit_control_data.h"

/* DB anonymous union _21A06478D43D3D5CB6DF9EA4457C77F5 (64 bytes) — _unit_datum member ___u87
 * at 0x284 (644): the most recent raw unit_control_data packet, viewed as sent-to-network or
 * received-from-network. $-name kept verbatim ($ in identifiers is an MSVC extension), matching
 * the mtlPARAM.h convention. */
typedef union _21A06478D43D3D5CB6DF9EA4457C77F5
{
    unit_control_data most_recent_control_data_for_network;  /* 0x00 */
    unit_control_data most_recent_control_data_from_network; /* 0x00 */
} _21A06478D43D3D5CB6DF9EA4457C77F5;

typedef struct _unit_datum
{
    int    actor_index;                            /* 0x000 */
    int    swarm_actor_index;                       /* 0x004 */
    int    swarm_next_unit_index;                   /* 0x008 */
    int    swarm_prev_unit_index;                   /* 0x00C (12) */
    unsigned int flags;                             /* 0x010 (16) */
    unsigned int control_flags;                     /* 0x014 (20) */
    int16_t timer;                                  /* 0x018 (24) */
    char   shield_sap_timeout;                      /* 0x01A (26) */
    char   magic_seat_index;                        /* 0x01B (27) */
    int    persistent_control_timer;                /* 0x01C (28) */
    unsigned int persistent_control_flags;          /* 0x020 (32) */
    int    player_index;                            /* 0x024 (36) — -1 when not player-controlled */
    int16_t last_unit_effect_type;                  /* 0x028 (40) */
    int16_t override_emotion_animation_index;       /* 0x02A (42) */
    int    game_time_at_last_unit_effect;           /* 0x02C (44) */
    real_vector3d desired_facing_vector;            /* 0x030 (48) */
    real_vector3d desired_aiming_vector;             /* 0x03C (60) */
    real_vector3d aiming_vector;                     /* 0x048 (72) */
    real_vector3d aiming_velocity;                   /* 0x054 (84) */
    real_vector3d desired_looking_vector;            /* 0x060 (96) */
    real_vector3d looking_vector;                    /* 0x06C (108) */
    real_vector3d looking_velocity;                  /* 0x078 (120) */
    real_vector3d throttle;                          /* 0x084 (132) */
    float  primary_trigger;                          /* 0x090 (144) */
    char   aiming_speed;                              /* 0x094 (148) */
    char   melee_attack_state;                        /* 0x095 (149) */
    char   melee_continuous_damage_effect_timer;       /* 0x096 (150) */
    uint8_t flaming_death_delay;               /* 0x097 (151) */
    char   weapon_drop_delay_ticks;                    /* 0x098 (152) */
    char   grenade_throw_state;                        /* 0x099 (153) */
    int16_t grenade_throw_ticks;                       /* 0x09A (154) */
    int16_t grenade_throw_full_power_ticks;            /* 0x09C (156) */
    unsigned char _pad09E[2];                          /* 0x09E (158) */
    int    grenade_object_index;                       /* 0x0A0 (160) */
    unit_animation animation;                          /* 0x0A4 (164) */
    float  ambient_illumination;                        /* 0x0EC (236) */
    float  self_illumination;                           /* 0x0F0 (240) */
    float  mouth_aperture;                              /* 0x0F4 (244) */
    int    last_entrance_attempt;                       /* 0x0F8 (248) */
    int16_t parent_seat_index;                          /* 0x0FC (252) */
    int16_t current_weapon_index;                       /* 0x0FE (254) */
    int16_t desired_weapon_index;                       /* 0x100 (256) */
    unsigned char _pad102[2];                           /* 0x102 (258) */
    int    weapon_object_indices[4];                    /* 0x104 (260) */
    int    weapon_last_used_at_game_time[4];            /* 0x114 (276) */
    int    equipment_object_index;                      /* 0x124 (292) */
    char   current_grenade_index;                       /* 0x128 (296) */
    char   desired_grenade_index;                       /* 0x129 (297) */
    char   grenade_counts[2];                           /* 0x12A (298) */
    char   current_zoom_level;                          /* 0x12C (300) */
    char   desired_zoom_level;                          /* 0x12D (301) */
    char   gunner_inactive_ticks;                       /* 0x12E (302) */
    uint8_t aiming_change;                      /* 0x12F (303) */
    int    driver_object_index;                         /* 0x130 (304) */
    int    gunner_object_index;                         /* 0x134 (308) */
    int    last_vehicle_index;                          /* 0x138 (312) */
    int    game_time_at_last_vehicle_exit;              /* 0x13C (316) */
    int16_t fake_encounter_index;                       /* 0x140 (320) */
    int16_t fake_squad_index;                           /* 0x142 (322) */
    float  seat_power[2];                                /* 0x144 (324) */
    float  integrated_light_power;                       /* 0x14C (332) */
    float  integrated_light_battery;                     /* 0x150 (336) */
    float  integrated_night_vision_power;                /* 0x154 (340) */
    real_point3d  seat_last_position;                    /* 0x158 (344) */
    real_vector3d seat_last_velocity;                    /* 0x164 (356) */
    real_vector3d seat_acceleration;                     /* 0x170 (368) */
    real_vector3d seat_desired_acceleration;             /* 0x17C (380) */
    float  active_camouflage;                            /* 0x188 (392) */
    float  active_camouflage_super_amount;               /* 0x18C (396) */
    int    dialogue_index;                               /* 0x190 (400) */
    unit_speech speech;                                  /* 0x194 (404) */
    int16_t last_damage_category;                        /* 0x210 (528) */
    int16_t delayed_damage_timer;                        /* 0x212 (530) */
    float  delayed_damage_peak;                          /* 0x214 (532) */
    int    delayed_damage_attacker_object_index;         /* 0x218 (536) */
    int    flaming_death_attacker_object_index;          /* 0x21C (540) */
    float  run_blindly_angle;                            /* 0x220 (544) */
    float  run_blindly_angle_delta;                      /* 0x224 (548) */
    int    time_of_death;                                /* 0x228 (552) */
    int16_t feign_death_timer;                           /* 0x22C (556) */
    int16_t cause_for_camo_regrowth;                     /* 0x22E (558) */
    float  body_stun;                                    /* 0x230 (560) */
    int16_t body_stun_ticks;                             /* 0x234 (564) */
    int16_t killing_spree_count;                         /* 0x236 (566) */
    int    killing_spree_last_time;                      /* 0x238 (568) */
    unit_attacker attackers[4];                          /* 0x23C (572) */
    int16_t user_animation_indices[2];                   /* 0x27C (636) */
    uint8_t force_local_update;                  /* 0x280 (640) */
    uint8_t is_from_network_data_valid;          /* 0x281 (641) */
    unsigned char _pad282[2];                            /* 0x282 (642) */
    _21A06478D43D3D5CB6DF9EA4457C77F5 ___u87;            /* 0x284 (644) — anonymous union, defined above */
    uint8_t did_just_complete_client_update;     /* 0x2C4 (708) */
    unsigned char _pad2C5[3];                            /* 0x2C5 (709) */
    int    last_completed_client_update_id;              /* 0x2C8 (712) */
    real_point3d position_after_completing_last_client_update; /* 0x2CC (716) */
} _unit_datum;                                            /* 728 bytes */
