#pragma once
/* continuous_vibrate_definition — sustained controller-vibration parameters embedded in a
 * continuous_damage_effect_definition ('cdmg' tag). Layout from the database (32 bytes). */

typedef struct continuous_vibrate_definition
{
    float        frequencies[2]; /* 0x00 — low/high vibration frequency scales */
    unsigned int unused[6];      /* 0x08 */
} continuous_vibrate_definition; /* 32 bytes */
