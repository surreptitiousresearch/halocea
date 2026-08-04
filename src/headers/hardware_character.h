#pragma once

typedef struct font_character font_character;

/* 8-byte cache slot; stored by value in the array, so it needs a full layout. */
typedef struct hardware_character
{
    font_character *character; /* 0x00 */
    short x0;                  /* 0x04 */
    short y0;                  /* 0x06 */
} hardware_character; /* 8 bytes */
