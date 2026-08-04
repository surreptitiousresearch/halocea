/* sound_environment — EAX-style reverb parameter set for a sound environment tag (sound_environments.c). */
#pragma once

typedef struct sound_environment
{
    int              pad1;                  /* 0x00 */
    __int16          priority;              /* 0x04 — higher wins when multiple environments apply */
    unsigned __int16 pad2;                  /* 0x06 */
    float            room_intensity;        /* 0x08 */
    float            room_intensity_hf;     /* 0x0C */
    float            room_rolloff_factor;   /* 0x10 */
    float            decay_time;            /* 0x14 */
    float            decay_hf_ratio;        /* 0x18 */
    float            reflections_intensity; /* 0x1C */
    float            reflections_delay;     /* 0x20 */
    float            reverb_intensity;      /* 0x24 */
    float            reverb_delay;          /* 0x28 */
    float            diffusion;             /* 0x2C */
    float            density;               /* 0x30 */
    float            hf_reference;          /* 0x34 */
    int              unused[4];             /* 0x38 */
} sound_environment;                        /* 72 bytes */

extern sound_environment default_sound_environment;
