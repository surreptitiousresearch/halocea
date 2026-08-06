#pragma once

#include <stdint.h>
#include "_D3DTAGCOLLECTION.h"
#include "_D3DConstants.h"
#include "GPU_DESTINATIONPACKET.h"

/* CALLING CONVENTION — decided 2026-08-06, do not re-propose. These vtable slots are plain,
 * with no convention token. The SDK spells COM methods STDMETHODCALLTYPE, which is __stdcall
 * on x86 and nothing on Xbox 360 PowerPC — and PowerPC has exactly one calling convention, so
 * on the actual target there is nothing to annotate. The slots are only ever called through
 * the pointer and never linked by name, so no symbol depends on it either. The `__fastcall`
 * these carried until the callconv drain was IDA's label for the PPC register ABI, not source.
 * Adding __stdcall back would be a fresh x86 claim on a PPC reconstruction. */
#include "GPU_WINDOWPACKET.h"
#include "GPU_VALUESPACKET.h"
#include "GPU_PROGRAMPACKET.h"
#include "GPU_CONTROLPACKET.h"
#include "GPU_TESSELLATORPACKET.h"
#include "GPU_MISCPACKET.h"
#include "GPU_POINTPACKET.h"

/* Xbox 360 D3D device (external library boundary type). Full DB layout (types_members
   D3DDevice, 20 members, total size 10880 / 0x2A80): pending-tag collection, ring-buffer
   pointers, the render/sampler state call tables, shader constants, and the GPU register
   packet blocks. Blam code touches the register-state call table at offset 0x40
   (m_SetRenderStateCall). */
typedef struct D3DDevice {
    _D3DTAGCOLLECTION m_Pending;                                                     /* 0x00 */
    uint64_t  m_Predicated_PendingMask2;                                     /* 0x28 */
    unsigned int     *m_pRing;                                                       /* 0x30 */
    unsigned int     *m_pRingLimit;                                                  /* 0x34 */
    unsigned int     *m_pRingGuarantee;                                              /* 0x38 */
    unsigned int      m_ReferenceCount;                                              /* 0x3C */
    void (*m_SetRenderStateCall[101])(struct D3DDevice *, unsigned int);             /* 0x40 */
    void (*m_SetSamplerStateCall[20])(struct D3DDevice *, unsigned int, unsigned int); /* 0x1D4 */
    unsigned int (*m_GetRenderStateCall[101])(struct D3DDevice *);                   /* 0x224 */
    unsigned int (*m_GetSamplerStateCall[20])(struct D3DDevice *, unsigned int); /* 0x3B8 */
    unsigned char     _pad408[120];                                                  /* 0x408 DB gap to 0x480 */
    _D3DConstants     m_Constants;                                                   /* 0x480 */
    float             m_ClipPlanes[6][4];                                            /* 0x2820 */
    GPU_DESTINATIONPACKET m_DestinationPacket;                                       /* 0x2880 */
    GPU_WINDOWPACKET  m_WindowPacket;                                                /* 0x28C0 */
    GPU_VALUESPACKET  m_ValuesPacket;                                                /* 0x28CC */
    GPU_PROGRAMPACKET m_ProgramPacket;                                               /* 0x2920 */
    GPU_CONTROLPACKET m_ControlPacket;                                               /* 0x2934 */
    GPU_TESSELLATORPACKET m_TessellatorPacket;                                       /* 0x2964 */
    GPU_MISCPACKET    m_MiscPacket;                                                  /* 0x29B8 */
    GPU_POINTPACKET   m_PointPacket;                                                 /* 0x2A50 */
} D3DDevice; /* 0x2A80 (10880) */
