#pragma once
/* continuous_player_effect_datum — runtime state for a sustained (looping) player effect's controller
 * vibration and camera shake. Layout from the database (16 bytes). */

typedef struct continuous_player_effect_datum
{
    float vibrate_frequencies[2]; /* 0x00 */
    float translational_shake;    /* 0x08 */
    float rotational_shake;       /* 0x0C */
} continuous_player_effect_datum; /* 0x10 */
