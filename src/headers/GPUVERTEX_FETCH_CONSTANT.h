#pragma once
#include <stdint.h>

/* GPUVERTEX_FETCH_CONSTANT - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPUVERTEX_FETCH_CONSTANT). */
typedef struct _E41E24A1CD47CDFA361AA9F99910948B { // DB types_members _E41E24A1CD47CDFA361AA9F99910948B
    uint32_t Type : 2;
    uint32_t BaseAddress : 30;
    uint32_t Endian : 2;
    uint32_t Size : 24;
    uint32_t AddressClamp : 1;
    uint32_t : 1;
    uint32_t RequestSize : 2;
    uint32_t ClampDisable : 2;
} _E41E24A1CD47CDFA361AA9F99910948B;

typedef union GPUVERTEX_FETCH_CONSTANT { // DB types_members GPUVERTEX_FETCH_CONSTANT
    _E41E24A1CD47CDFA361AA9F99910948B __s0; /* 0x00 */
    unsigned int dword[2]; /* 0x00 */
} GPUVERTEX_FETCH_CONSTANT;
