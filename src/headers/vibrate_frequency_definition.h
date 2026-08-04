#pragma once
/* vibrate_frequency_definition — one rumble frequency channel. Layout from the database. */

typedef struct vibrate_frequency_definition
{
    float            frequency;     /* 0x00 — amplitude of this channel */
    float            duration;      /* 0x04 — seconds the impulse lasts */
    __int16          fade_function; /* 0x08 — transition function selector */
    unsigned __int16 pad;           /* 0x0A */
    unsigned int     unused[2];     /* 0x0C */
} vibrate_frequency_definition;     /* 20 bytes */
