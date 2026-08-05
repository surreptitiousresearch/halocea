#pragma once
#include <stdint.h>

/* Decoded S3TC texel color (RGBA8). */

typedef struct S3TC_COLOR
{
    uint8_t rgba[4]; /* 0x00 */
} S3TC_COLOR; /* 4 bytes */
