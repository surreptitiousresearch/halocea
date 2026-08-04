#pragma once

#include "_D3DDEVTYPE.h"
#include "_D3DVSHADERCAPS2_0.h"
#include "_D3DPSHADERCAPS2_0.h"

/* Xbox 360 D3D device-capabilities struct (external library boundary type). Full DB-verified
   304-byte layout. VS20Caps/PS20Caps are the SDK sub-structs _D3DVSHADERCAPS2_0 (16b) /
   _D3DPSHADERCAPS2_0 (20b), typed per DB. */
typedef struct _D3DCAPS9
{
    _D3DDEVTYPE  DeviceType;                        /* 0x00 */
    unsigned int AdapterOrdinal;                    /* 0x04 */
    unsigned int Caps;                              /* 0x08 */
    unsigned int Caps2;                             /* 0x0C */
    unsigned int Caps3;                             /* 0x10 */
    unsigned int PresentationIntervals;            /* 0x14 */
    unsigned int CursorCaps;                        /* 0x18 */
    unsigned int DevCaps;                           /* 0x1C */
    unsigned int PrimitiveMiscCaps;                 /* 0x20 */
    unsigned int RasterCaps;                        /* 0x24 */
    unsigned int ZCmpCaps;                          /* 0x28 */
    unsigned int SrcBlendCaps;                      /* 0x2C */
    unsigned int DestBlendCaps;                     /* 0x30 */
    unsigned int AlphaCmpCaps;                      /* 0x34 */
    unsigned int ShadeCaps;                         /* 0x38 */
    unsigned int TextureCaps;                       /* 0x3C */
    unsigned int TextureFilterCaps;                 /* 0x40 */
    unsigned int CubeTextureFilterCaps;             /* 0x44 */
    unsigned int VolumeTextureFilterCaps;           /* 0x48 */
    unsigned int TextureAddressCaps;                /* 0x4C — D3DPTADDRESSCAPS (bit 3 = border-address support) */
    unsigned int VolumeTextureAddressCaps;          /* 0x50 */
    unsigned int LineCaps;                          /* 0x54 */
    unsigned int MaxTextureWidth;                   /* 0x58 */
    unsigned int MaxTextureHeight;                  /* 0x5C */
    unsigned int MaxVolumeExtent;                   /* 0x60 */
    unsigned int MaxTextureRepeat;                  /* 0x64 */
    unsigned int MaxTextureAspectRatio;             /* 0x68 */
    unsigned int MaxAnisotropy;                     /* 0x6C */
    float        MaxVertexW;                        /* 0x70 */
    float        GuardBandLeft;                     /* 0x74 */
    float        GuardBandTop;                      /* 0x78 */
    float        GuardBandRight;                    /* 0x7C */
    float        GuardBandBottom;                   /* 0x80 */
    float        ExtentsAdjust;                     /* 0x84 */
    unsigned int StencilCaps;                       /* 0x88 */
    unsigned int FVFCaps;                           /* 0x8C */
    unsigned int TextureOpCaps;                     /* 0x90 */
    unsigned int MaxTextureBlendStages;             /* 0x94 */
    unsigned int MaxSimultaneousTextures;           /* 0x98 */
    unsigned int VertexProcessingCaps;              /* 0x9C */
    unsigned int MaxActiveLights;                   /* 0xA0 */
    unsigned int MaxUserClipPlanes;                 /* 0xA4 */
    unsigned int MaxVertexBlendMatrices;            /* 0xA8 */
    unsigned int MaxVertexBlendMatrixIndex;         /* 0xAC */
    float        MaxPointSize;                      /* 0xB0 */
    unsigned int MaxPrimitiveCount;                 /* 0xB4 */
    unsigned int MaxVertexIndex;                    /* 0xB8 */
    unsigned int MaxStreams;                        /* 0xBC */
    unsigned int MaxStreamStride;                   /* 0xC0 */
    unsigned int VertexShaderVersion;               /* 0xC4 */
    unsigned int MaxVertexShaderConst;              /* 0xC8 */
    unsigned int PixelShaderVersion;                /* 0xCC — D3DPSHADERCAPS */
    float        PixelShader1xMaxValue;             /* 0xD0 */
    unsigned int DevCaps2;                          /* 0xD4 */
    float        MaxNpatchTessellationLevel;        /* 0xD8 */
    unsigned int Reserved5;                         /* 0xDC */
    unsigned int MasterAdapterOrdinal;              /* 0xE0 */
    unsigned int AdapterOrdinalInGroup;             /* 0xE4 */
    unsigned int NumberOfAdaptersInGroup;           /* 0xE8 */
    unsigned int DeclTypes;                         /* 0xEC */
    unsigned int NumSimultaneousRTs;                /* 0xF0 */
    unsigned int StretchRectFilterCaps;             /* 0xF4 */
    _D3DVSHADERCAPS2_0 VS20Caps;                    /* 0xF8 */
    _D3DPSHADERCAPS2_0 PS20Caps;                    /* 0x108 */
    unsigned int VertexTextureFilterCaps;           /* 0x11C */
    unsigned int MaxVShaderInstructionsExecuted;    /* 0x120 */
    unsigned int MaxPShaderInstructionsExecuted;    /* 0x124 */
    unsigned int MaxVertexShader30InstructionSlots; /* 0x128 */
    unsigned int MaxPixelShader30InstructionSlots;  /* 0x12C */
} _D3DCAPS9; /* 304 bytes */
