#pragma once
/* ai_globals @ ai_globals — root AI runtime state (ai.c), 2268 bytes. Partial: the per-frame control
 * flags, service-timer accounting, dialogue timers and the mounted-weapon placement queue accessed by
 * the AI tick are named at their true offsets (verified against the DB); the rest is opaque. */

#include <stdint.h>
#include "recent_conversation.h"
#include "ai_vehicle_enterable.h"
#include "ai_spatial_effect.h"

typedef struct ai_globals_t
{
    uint8_t ai_active;                      /* 0x00 */
    uint8_t ai_initialized_for_map;         /* 0x01 */
    uint8_t ai_has_control_data;            /* 0x02 */
    uint8_t time_given_this_frame;          /* 0x03 */
    int16_t         last_highest_service_timer;     /* 0x04 */
    int16_t         current_highest_service_timer;  /* 0x06 */
    int             first_encounterless_actor_index;/* 0x08 */
    float           major_upgrade_error;            /* 0x0C */
    uint8_t dialogue_triggers_enabled;      /* 0x10 */
    unsigned char   _pad11[3];                      /* 0x11 */
    int             last_chatter_time[2];           /* 0x14 */
    int             last_talk_time[2];              /* 0x1C */
    int             last_shout_time[2];             /* 0x24 */
    int16_t         recent_conversation_count;      /* 0x2C */
    int16_t         recent_conversation_next_index; /* 0x2E */
    recent_conversation recent_conversations[16];   /* 0x30 */
    int16_t         spatial_effects_first_index;    /* 0x130 */
    int16_t         spatial_effects_last_index;     /* 0x132 */
    ai_spatial_effect spatial_effects[32];           /* 0x134 — AI noise/stimulus ring buffer */
    uint8_t grenades_enabled;               /* 0x3B4 */
    unsigned char   _pad3B5;                         /* 0x3B5 */
    int16_t         enterable_vehicle_count;        /* 0x3B6 */
    ai_vehicle_enterable enterable_vehicles[32];     /* 0x3B8 — script-registered AI-enterable vehicles */
    int16_t         mounted_weapon_unit_count;      /* 0x8B8 */
    int16_t         _pad8BA;                         /* 0x8BA */
    int             mounted_weapon_unit_indices[8]; /* 0x8BC */
} ai_globals_t;                                     /* 2268 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern ai_globals_t *ai_globals;

#ifdef __cplusplus
}
#endif
