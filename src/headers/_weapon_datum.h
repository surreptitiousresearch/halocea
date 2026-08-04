#pragma once
/* _weapon_datum — the weapon-layer runtime payload embedded at offset 0x22C in weapon_datum,
 * after the shared _object_datum (0x004) and _item_datum (0x1F4) layers.
 * Layout DB-verified against type '_weapon_datum' (276 bytes, 2026-07-14). */

#include "weapon_trigger.h"
#include "weapon_magazine.h"
#include "animation_state.h"
#include "weapon_datum_network_data.h"

typedef struct _weapon_datum
{
    unsigned int     flags;                        /* 0x00 */
    unsigned __int16 control_flags;                /* 0x04 */
    unsigned __int16 pad;                          /* 0x06 */
    float            primary_trigger;              /* 0x08 */
    char             state;                        /* 0x0C */
    char             last_reported_state;          /* 0x0D */
    __int16          state_timer;                  /* 0x0E */
    float            heat;                         /* 0x10 */
    float            age;                          /* 0x14 */
    float            overcharged;                  /* 0x18 */
    float            integrated_light_power;       /* 0x1C */
    char             integrated_light_delay_ticks; /* 0x20 */
    char             pad1[3];                      /* 0x21 */
    int              tracked_object_index;         /* 0x24 */
    float            recoil_angular_velocity;      /* 0x28 */
    __int16          recoil_recovery_time;         /* 0x2C */
    __int16          shots_until_demotion;         /* 0x2E */
    __int16          alternate_shots_loaded;       /* 0x30 */
    __int16          pad2;                         /* 0x32 */
    weapon_trigger   triggers[2];                  /* 0x34 */
    weapon_magazine  magazines[2];                 /* 0x84 */
    animation_state  animation;                    /* 0x9C */
    int              overheated_effect_index;      /* 0xA0 */
    int              game_time_last_fired;         /* 0xA4 */
    __int16          magazine_rounds_total_at_reload_start[2];        /* 0xA8 */
    __int16          magazine_rounds_loaded_at_reload_start[2];       /* 0xAC */
    __int16          magazine_rounds_total_to_use_at_reload_finish[2];/* 0xB0 */
    unsigned __int8  baseline_valid;               /* 0xB4 */
    unsigned __int8  baseline_index;               /* 0xB5 */
    unsigned __int8  message_index;                /* 0xB6 */
    unsigned __int8  pad3;                         /* 0xB7 */
    weapon_datum_network_data baseline;            /* 0xB8 */
    unsigned __int8  last_network_data_valid;      /* 0xE4 */
    unsigned __int8  pad4[3];                      /* 0xE5 */
    weapon_datum_network_data last_network_data;   /* 0xE8 */
} _weapon_datum;                                   /* 276 bytes */
