#pragma once
#include <stdint.h>
/* location — a position within the structure BSP (cluster + leaf). */

typedef struct location
{
    int     leaf_index;    /* 0x0 */
    int16_t cluster_index; /* 0x4 — 0xFFFF when none */
    uint16_t bonus;/* 0x6 */
} location;                /* 8 bytes */
