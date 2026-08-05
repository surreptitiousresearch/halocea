#pragma once
#include <stdint.h>
/* king_variant — king-of-the-hill game-engine variant settings (1 byte, DB layout). */

typedef struct king_variant
{
    uint8_t moving_hill; /* 0x0 */
} king_variant;                  /* 1 byte */
