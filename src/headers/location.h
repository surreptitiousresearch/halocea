#pragma once
/* location — a position within the structure BSP (cluster + leaf). */

typedef struct location
{
    int     leaf_index;    /* 0x0 */
    __int16 cluster_index; /* 0x4 — 0xFFFF when none */
    unsigned __int16 bonus;/* 0x6 */
} location;                /* 8 bytes */
