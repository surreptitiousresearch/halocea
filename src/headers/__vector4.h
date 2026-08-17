#pragma once

/* __vector4 - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members __vector4). */
typedef struct _91D1B2149FAC90180ECB9AC277F76009 { // DB types_members _91D1B2149FAC90180ECB9AC277F76009
    float x; /* 0x00 */
    float y; /* 0x04 */
    float z; /* 0x08 */
    float w; /* 0x0C */
} _91D1B2149FAC90180ECB9AC277F76009;

typedef union _967C8B9C0D9207D1E708619458A2B618 { // DB types_members _967C8B9C0D9207D1E708619458A2B618
    float vector4_f32[4]; /* 0x00 */
    unsigned int vector4_u32[4]; /* 0x00 */
    _91D1B2149FAC90180ECB9AC277F76009 __s2; /* 0x00 */
    float v[4]; /* 0x00 */
    unsigned int u[4]; /* 0x00 */
} _967C8B9C0D9207D1E708619458A2B618;

/* 16-byte aligned like the SDK's __declspec(align(16)) __vector4 (a VMX128 register
   image); without it every record embedding one under-pads its tail vs the DB
   (hkpLinearCastCollisionInput measured 108 vs DB 112). */
typedef struct __declspec(align(16)) __vector4 { // DB types_members __vector4
    _967C8B9C0D9207D1E708619458A2B618 ___u0; /* 0x00 */
} __vector4;
