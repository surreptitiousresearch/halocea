#pragma once
/* unit_hud_state — runtime per-local-player HUD state for the unit HUD. Layout matches the
 * database exactly. */

typedef struct unit_hud_state
{
    float          last_shield_vitality;         /* 0x00 */
    float          last_body_vitality;           /* 0x04 */
    float          fade_time;                    /* 0x08 */
    int            last_shield_hit_time;         /* 0x0C */
    int            last_shield_flash_time;       /* 0x10 */
    int            last_health_flash_time;       /* 0x14 */
    int            last_motion_sensor_flash_time;/* 0x18 */
    int            last_unit_index;              /* 0x1C */
    unsigned short auxilary_active_type_flags;   /* 0x20 */
    short          auxilary_flash_time[1];       /* 0x22 */
    unsigned short sound_flags;                  /* 0x24 */
    char           _pad26[2];                    /* 0x26 */
    int            last_sound_handles[12];       /* 0x28 */
} unit_hud_state;                                /* 88 bytes */
