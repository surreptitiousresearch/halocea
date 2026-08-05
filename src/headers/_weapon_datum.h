#pragma once
/* _weapon_datum — the weapon-layer runtime payload embedded at offset 0x22C in weapon_datum,
 * after the shared _object_datum (0x004) and _item_datum (0x1F4) layers.
 * Layout DB-verified against type '_weapon_datum' (276 bytes, 2026-07-14). */

#include <stdint.h>
#include "weapon_trigger.h"
#include "weapon_magazine.h"
#include "animation_state.h"
#include "weapon_datum_network_data.h"

typedef struct _weapon_datum
{
    unsigned int     flags;                        /* 0x00 */
    uint16_t control_flags;                /* 0x04 */
    uint16_t pad;                          /* 0x06 */
    float            primary_trigger;              /* 0x08 */
    char             state;                        /* 0x0C */
    char             last_reported_state;          /* 0x0D */
    int16_t          state_timer;                  /* 0x0E */
    float            heat;                         /* 0x10 */
    float            age;                          /* 0x14 */
    float            overcharged;                  /* 0x18 */
    float            integrated_light_power;       /* 0x1C */
    char             integrated_light_delay_ticks; /* 0x20 */
    char             pad1[3];                      /* 0x21 */
    int              tracked_object_index;         /* 0x24 */
    float            recoil_angular_velocity;      /* 0x28 */
    int16_t          recoil_recovery_time;         /* 0x2C */
    int16_t          shots_until_demotion;         /* 0x2E */
    int16_t          alternate_shots_loaded;       /* 0x30 */
    int16_t          pad2;                         /* 0x32 */
    weapon_trigger   triggers[2];                  /* 0x34 */
    weapon_magazine  magazines[2];                 /* 0x84 */
    animation_state  animation;                    /* 0x9C */
    int              overheated_effect_index;      /* 0xA0 */
    int              game_time_last_fired;         /* 0xA4 */
    int16_t          magazine_rounds_total_at_reload_start[2];        /* 0xA8 */
    int16_t          magazine_rounds_loaded_at_reload_start[2];       /* 0xAC */
    int16_t          magazine_rounds_total_to_use_at_reload_finish[2];/* 0xB0 */
    uint8_t  baseline_valid;               /* 0xB4 */
    uint8_t  baseline_index;               /* 0xB5 */
    uint8_t  message_index;                /* 0xB6 */
    uint8_t  pad3;                         /* 0xB7 */
    weapon_datum_network_data baseline;            /* 0xB8 */
    uint8_t  last_network_data_valid;      /* 0xE4 */
    uint8_t  pad4[3];                      /* 0xE5 */
    weapon_datum_network_data last_network_data;   /* 0xE8 */
} _weapon_datum;                                   /* 276 bytes */
