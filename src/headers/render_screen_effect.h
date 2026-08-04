#pragma once

typedef struct render_screen_effect
{
    short type;          // 0x00
    unsigned char _pad0[2]; /* db-verified padding */
    /* 0x02..0x03 padding */
    float intensity;     // 0x04
} render_screen_effect;  // 8 bytes
