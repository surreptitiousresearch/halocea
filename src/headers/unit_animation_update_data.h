#pragma once
/* unit_animation_update_data — small (2-byte) scratch handed to the biped movement / animation update.
 * byte[0] is the desired animation state, byte[1] the crouch flag (recovered from call sites). */

typedef struct unit_animation_update_data
{
    char state_desired;             /* 0x0 */
    unsigned __int8 crouching;      /* 0x1 */
} unit_animation_update_data;       /* 2 bytes */
