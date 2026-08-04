#pragma once
/* hcex_float3 — plain 3-float vector used across the HCEX bridge layer (hcex). */

typedef struct hcex_float3
{
    float x;   /* 0x00 */
    float y;   /* 0x04 */
    float z;   /* 0x08 */
} hcex_float3;  /* 12 bytes */
