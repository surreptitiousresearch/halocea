#pragma once
/* looping_sound_scale_modifiers — scale-bound modifiers in a 'lsnd' tag (sound.c).
 * 12 bytes, DB types_members-confirmed. */

typedef struct looping_sound_scale_modifiers
{
    float detail_period; /* 0x00 */
    int   unused0[2];    /* 0x04 */
} looping_sound_scale_modifiers;
