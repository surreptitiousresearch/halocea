#pragma once
/* Boundary declarations for hcex_fill_backbuffer.c.
 *
 * hcex_fill_backbuffer runs the final gamma/tonemap pass: it configures a PASS_DESC_FILL from the video
 * driver's gamma settings and the HDR render target, then draws a full-screen textured quad through the ws
 * render driver. Every driver/manager it touches (d3d/vid drivers, txm manager, rend driver) is a large
 * self-contained ws-engine class treated as an extern boundary; only the fields the pass reads/writes are
 * modeled, at their database offsets (verified via types_members). Virtual-dispatch calls are declared as
 * free functions taking the object as the first parameter (the corpus convention). */

/* --- canonical DB-verified full layouts (the former per-consumer partial slices were
 * consolidated): m3d colour, txm texture/manager, vid mode/driver, d3d driver --- */
#include "../ws/m3d/m3dCOLOR.h"
#include "../ws/txm/txmTEXTURE.h"
#include "../ws/txm/txmTEXTURE_PTR.h"
#include "../ws/txm/txmMANAGER.h"
#include "../ws/txm/txmD3D_TEX.h"
#include "../vidMODE.h"
#include "../vidDRIVER.h"
#include "../d3d_driver.h"

typedef vidGAMMA_CONFIG vidGAMMA_CFG; /* legacy spelling of the driver's gamma block */

/* --- PASS_DESC_FILL (60 bytes, DB layout) — gamma/fill pass descriptor --- */
typedef struct PASS_DESC_FILL
{
    /* Nested 1-byte enums — DB-verbatim enumerator sets (types_enum_values PASS_DESC_FILL::*).
     * `: unsigned char` is load-bearing. A plain C++ enum is 4 bytes, which pushed writeColor,
     * colorConst and all four flags off their DB offsets and sized the block 76 instead of 60 —
     * the comments below always claimed 1 byte, but nothing enforced it.
     * NOTE: the previous boundary spelling wrongly defined PASS_GAMMA as 5; the DB has
     * PASS_GAMMA = 0 (5 is PASS_MSAA_RESOLVE_AND_REMAP), and the binary stores 0 here. */
    enum PASS : unsigned char {
        PASS_GAMMA = 0, PASS_GAMMA_BICUBIC0 = 1, PASS_GAMMA_BICUBIC1 = 2,
        PASS_MSAA_RESOLVE = 3, PASS_MSAA_REMAP = 4, PASS_MSAA_RESOLVE_AND_REMAP = 5,
        PASS_DEPTH_DOWNSAMPLE = 6, PASS_FILL = 7, PASS_FILL_MAX_Z = 8,
        PASS_FILL_CONV_Z = 9, PASS_RESTORE_HIGH_Z = 10,
    };
    enum XENONMSAARESOLVETYPE : unsigned char {
        XENONMSAARESOLVETYPE_ONE_X = 0, XENONMSAARESOLVETYPE_TWO_X = 1,
        XENONMSAARESOLVETYPE_FOUR_X = 2,
    };
    enum XENONMSAARESOLVEFLAGS : unsigned char {
        XENONMSAARESOLVEFLAGS_MIN_DEPTH = 0, XENONMSAARESOLVEFLAGS_MAX_DEPTH = 1,
    };
    enum DEPTHDOWNSAMPLEFLAGS : unsigned char {
        DEPTHDOWNSAMPLEFLAGS_MIN_DEPTH = 0, DEPTHDOWNSAMPLEFLAGS_MAX_DEPTH = 1,
    };

    PASS           pass;                  /* 0x00 (1 byte) */
    bool           writeColor;            /* 0x01 */
    unsigned char  _pad02[2];             /* 0x02 db-verified padding */
    m3dCOLOR       colorConst;            /* 0x04 */
    bool           writeDepth;            /* 0x14 */
    XENONMSAARESOLVETYPE  xenonMSAAResolveType;  /* 0x15 (1 byte) */
    XENONMSAARESOLVEFLAGS xenonMSAAResolveFlags; /* 0x16 (1 byte) */
    DEPTHDOWNSAMPLEFLAGS  depthDownsampleFlags;  /* 0x17 (1 byte) */
    bool           depthDownsampleNormals;/* 0x18 */
    unsigned char  _pad19[3];             /* 0x19 db-verified padding */
    txmTEXTURE_PTR texColor;              /* 0x1C */
    txmTEXTURE_PTR texColor1;             /* 0x20 */
    txmTEXTURE_PTR texDepth;              /* 0x24 */
    txmTEXTURE_PTR texDepth1;             /* 0x28 */
    txmTEXTURE_PTR texDepth2;             /* 0x2C */
    txmTEXTURE_PTR texDepth3;             /* 0x30 */
    txmTEXTURE_PTR texGamma;              /* 0x34 */
    txmTEXTURE_PTR texBicubicLookup;      /* 0x38 */
} PASS_DESC_FILL;                          /* 60 bytes */

typedef struct rendDRIVER rendDRIVER;   /* opaque */

/* --- globals --- */
extern d3dDRIVER   *d3dDriver;
extern vidDRIVER   *vidDriver;
extern txmMANAGER  *txmManager;
extern rendDRIVER  *rendDrv;
extern txmD3D_TEX **hcxe_global_render_targets_tex;

/* --- ws-engine methods (free-function form; several are virtual dispatch) --- */
extern void d3dDRIVER_SetStartRenderingState(d3dDRIVER *self);
extern int  osLockedIncrement(int *value);
extern void rendDRIVER_Configure(rendDRIVER *self, int count, PASS_DESC_FILL *desc,
        unsigned long long coordDescriptor, int renderBlock, int arg5, int arg6);   /* virtual +0x40 */
extern void rendDRIVER_DrawPoly(rendDRIVER *self, void *vertices, int nVert,
        unsigned long long coordDescriptor);
extern void PASS_DESC_FILL_dtor(PASS_DESC_FILL *self);
extern void *memset(void *dst, int c, unsigned int n);
