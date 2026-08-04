#pragma once

/* GPUVERTEX_FETCH_CONSTANT - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPUVERTEX_FETCH_CONSTANT). */
typedef struct _E41E24A1CD47CDFA361AA9F99910948B { // DB types_members _E41E24A1CD47CDFA361AA9F99910948B
    unsigned __int32 Type : 2;
    unsigned __int32 BaseAddress : 30;
    unsigned __int32 Endian : 2;
    unsigned __int32 Size : 24;
    unsigned __int32 AddressClamp : 1;
    unsigned __int32 : 1;
    unsigned __int32 RequestSize : 2;
    unsigned __int32 ClampDisable : 2;
} _E41E24A1CD47CDFA361AA9F99910948B;

typedef union GPUVERTEX_FETCH_CONSTANT { // DB types_members GPUVERTEX_FETCH_CONSTANT
    _E41E24A1CD47CDFA361AA9F99910948B __s0; /* 0x00 */
    unsigned int dword[2]; /* 0x00 */
} GPUVERTEX_FETCH_CONSTANT;
