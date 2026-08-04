#pragma once

/* GPU_COLORMASK - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_COLORMASK). */
typedef struct _4C97A0E9DB870CE221244CD296E9934B { // DB types_members _4C97A0E9DB870CE221244CD296E9934B
    unsigned __int32 Write0 : 4;
    unsigned __int32 Write1 : 4;
    unsigned __int32 Write2 : 4;
    unsigned __int32 Write3 : 4;
} _4C97A0E9DB870CE221244CD296E9934B;

typedef union GPU_COLORMASK { // DB types_members GPU_COLORMASK
    _4C97A0E9DB870CE221244CD296E9934B __s0; /* 0x00 */
    unsigned int dword; /* 0x00 */
} GPU_COLORMASK;
