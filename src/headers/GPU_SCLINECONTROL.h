#pragma once

/* GPU_SCLINECONTROL - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_SCLINECONTROL). */
typedef struct _8F9DF130A6CCCA69B8D6DFFCF09200E1 { // DB types_members _8F9DF130A6CCCA69B8D6DFFCF09200E1
    unsigned __int32 BresControl : 8;
    unsigned __int32 UseBresControl : 1;
    unsigned __int32 ExpandLineWidth : 1;
    unsigned __int32 LastPixel : 1;
} _8F9DF130A6CCCA69B8D6DFFCF09200E1;

typedef union GPU_SCLINECONTROL { // DB types_members GPU_SCLINECONTROL
    _8F9DF130A6CCCA69B8D6DFFCF09200E1 __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_SCLINECONTROL;
