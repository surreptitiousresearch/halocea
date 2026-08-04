#pragma once
/* sound_scale_modifiers — per-bound scale modifiers for a 'snd!' sound tag (sound.c). Layout verbatim from the DB. */

typedef struct sound_scale_modifiers
{
    float skip_fraction; /* 0x00 */
    float gain;          /* 0x04 */
    float pitch;         /* 0x08 */
    int   unused0[3];    /* 0x0C */
} sound_scale_modifiers; /* 24 bytes */
