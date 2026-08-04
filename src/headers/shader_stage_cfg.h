#pragma once

/* One texture-stage config (6 bytes) for the fixed-function "chicago" emulator. */
typedef struct shader_stage_cfg
{
    short color_func;  /* 0x00 */
    short alpha_func;  /* 0x02 */
    short a_replicate; /* 0x04 */
} shader_stage_cfg; /* 6 bytes */
