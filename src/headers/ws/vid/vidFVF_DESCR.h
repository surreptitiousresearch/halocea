#pragma once
// ws-engine vid: flexible-vertex-format descriptor. DB-verified layout (types_members
// vidFVF_DESCR, size 148): derives vidFVF_DESCR_BASE@0 (vtable + the three per-variant attribute
// masks), then the four 64-bit fvf bitmasks (multistream/interleaved/instanced/full), the full
// attribute mask + attribute table, stream mask/strides, descriptor number and packed size.
// Cached and reused by vidVBUF_MNG::FitFVF. Most methods are virtual boundary slots.

#include "../ap/apSTATE_T.h"

struct vidFVF_DESCR_BASE_vtbl; // boundary -- descriptor vtable

// DB-verified (types_members vidFVF_ATTR, size 5). Field spellings preserved verbatim from DB.
typedef struct vidFVF_ATTR
{
    unsigned char dimemsion; /* 0x00 (DB spelling) */
    unsigned char size;      /* 0x01 */
    unsigned char stream;    /* 0x02 */
    unsigned char offset;    /* 0x03 */
    unsigned char stride;    /* 0x04 */
} vidFVF_ATTR;

// DB-verified (types_members vidFVF_DESCR_BASE, size 12).
typedef struct vidFVF_DESCR_BASE
{
    vidFVF_DESCR_BASE_vtbl   *__vftable;                 /* 0x00 */
    apSTATE_T<unsigned short> attributeMaskMultistream;  /* 0x04 */
    apSTATE_T<unsigned short> attributeMaskInteleaved;   /* 0x06 (DB spelling) */
    apSTATE_T<unsigned short> attributeMaskInstanced;    /* 0x08 */
    unsigned char             _pad0A[2];                 /* 0x0A */
} vidFVF_DESCR_BASE;

typedef struct vidFVF_DESCR : vidFVF_DESCR_BASE
{
    unsigned long long        fvf_multistream;   /* 0x0C */
    unsigned long long        fvf_interleaved;   /* 0x14 */
    unsigned long long        fvf_instanced;     /* 0x1C */
    unsigned long long        fvf_full;          /* 0x24 */
    apSTATE_T<unsigned short> attributeMaskFull; /* 0x2C */
    vidFVF_ATTR               attirbutes[16];    /* 0x2E (DB spelling) */
    apSTATE_T<unsigned short> streamMask;        /* 0x7E */
    unsigned char             streamStride[16];  /* 0x80 */
    unsigned short            nmb;               /* 0x90 */
    unsigned char             sizeOfFVF;         /* 0x92 */
    unsigned char             _pad93;            /* 0x93 */

    // vtbl slot 3 (offset 0xC), virtual (UAA). Normalize+store the four raw fvf masks and derive
    // the attribute/stream tables. boundary body (external to this batch;
    // ?Init@vidFVF_DESCR@@UAAX_K000@Z).
    void Init(unsigned long long full, unsigned long long multistream,
              unsigned long long interleaved, unsigned long long instanced);

    // vtbl slot 1 (offset 0x4), virtual. Build the platform vertex declaration (in the d3d build
    // the resulting D3DVertexDeclaration* is stored immediately past the object at +0x94). boundary.
    void CreateDecl();

    vidFVF_DESCR();   // ?vidFVF_DESCR@@QAA@XZ  boundary
    ~vidFVF_DESCR();  // boundary
} vidFVF_DESCR; /* 148 bytes */
