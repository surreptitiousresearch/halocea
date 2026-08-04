#pragma once

#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "ENGINE_TEX_DESC.h"

/* txmTEXTURE -- ws-engine texture-manager base class (txm subsystem). DB-verified layout
 * (types_members txmTEXTURE, size 0x94 (148)). Per [[re-source-boundary-conventions]] only the
 * members actually touched by the txm reconstructions in this batch are typed; everything else
 * is raw padding at its exact DB offset/size so sizeof() and downstream offsets (e.g. the
 * derived txmD3D_TEX fields) stay correct without pulling in the pct/vid/strm subsystems.
 * sizeX/sizeY/desc/rtWidth/rtHeight/swizzle were added by the GetRTSizeX/Y + SetDesc +
 * UpdateTextureSwizzle reconstructions (DB-verified offsets 0x08/0x0A/0x14/0x5C/0x60/0x8C). */

typedef enum TXM_MULTISAMPLE
{
    TXM_MULTISAMPLE_NONE = 0,
    TXM_MULTISAMPLE_2X   = 1,
    TXM_MULTISAMPLE_4X   = 2,
} TXM_MULTISAMPLE;

// Swizzle-source selector consumed by SetSwizzle (R/G/B/A/const-0-or-1 channel index into a
// 4-channel source-select word). DB types_enum_values TXM_TEX_SWIZZLE_SRC.
typedef enum TXM_TEX_SWIZZLE_SRC
{
    TXM_TEX_SWIZZLE_SRC_X = 0,
    TXM_TEX_SWIZZLE_SRC_Y = 1,
    TXM_TEX_SWIZZLE_SRC_Z = 2,
    TXM_TEX_SWIZZLE_SRC_W = 3,
    TXM_TEX_SWIZZLE_SRC_0 = 4,
    TXM_TEX_SWIZZLE_SRC_1 = 5,
} TXM_TEX_SWIZZLE_SRC;

struct strmRESOURCE_TEXTURE; // ../strm/strmRESOURCE_TEXTURE.h — forward decl only, avoids pulling strm* in here
struct txmTEXTURE_vtbl;      // boundary — full slot table not expanded; only the slots below are documented
struct pctPICTURE;           // pct subsystem — boundary (pointer only)
struct vidDEFRAG_CHUNK;      // ../vid/vidDEFRAG_CHUNK.h — boundary (pointer only)

typedef struct txmTEXTURE
{
    txmTEXTURE_vtbl         *__vftable;     /* 0x000 */
    dsTSTRING<char>          name;          /* 0x004 */
    short                    sizeX;         /* 0x008 */
    short                    sizeY;         /* 0x00A */
    short                    sizeZ;         /* 0x00C */
    char                     nFaces;        /* 0x00E */
    char                     nMipMap;       /* 0x00F */
    char                     nSkipedMips;   /* 0x010 (DB spelling kept verbatim) */
    unsigned char            _pad011[1];    /* 0x011 */
    short                    fmtNmb;        /* 0x012 */
    ENGINE_TEX_DESC          desc;          /* 0x014 */
    short                    orderNmb;      /* 0x04C */
    short                    uniqueID;      /* 0x04E -- DB-verified (types_members txmTEXTURE) */
    int                      refCount;      /* 0x050 */
    char                     rtFormat;      /* 0x054 */
    unsigned char            _pad055[3];    /* 0x055 */
    TXM_MULTISAMPLE          rtMultisample; /* 0x058 */
    int                      rtWidth;       /* 0x05C */
    int                      rtHeight;      /* 0x060 */
    int                      frameNumUpd;   /* 0x064 */
    unsigned char           *pData;         /* 0x068 */
    unsigned short           pitchX;        /* 0x06C */
    unsigned char            _pad06E[2];    /* 0x06E */
    int                      texSize;       /* 0x070 */
    vidDEFRAG_CHUNK         *pChunk;        /* 0x074 */
    apSTATE_T<unsigned long> state;         /* 0x078 */
    apSTATE_T<unsigned long> stateFltr;     /* 0x07C */
    pctPICTURE              *pPictSrc;      /* 0x080 -- boundary (pointer only) */
    txmTEXTURE              *pSplitScreenTex[2]; /* 0x084 */
    unsigned short           swizzle;       /* 0x08C */
    unsigned char            _pad08E[2];    /* 0x08E boundary padding */
    strmRESOURCE_TEXTURE    *strmResource;  /* 0x090 -- DB-verified (types_members txmTEXTURE) */

    ~txmTEXTURE(); // boundary -- dtr_txmTEXTURE (vtable slot 0; vptr modeled as __vftable above)

    // vtable slot 0x34 -- locate the texture's tag/pak entry on disk given the manager's
    // search path/extension list; writes the resolved path into outPath. boundary (not
    // reversed in this batch; called from txmMANAGER::Load).
    int FindDisk(const char *path, const char *ext, char *outPath);

    // vtable slot 0x38 -- load pixel data from the resolved disk path. boundary (not decompiled
    // in this batch; called from txmMANAGER::Load).
    int Load(const char *path, const char *ext);

    // vtable slot 0x94 (148) -- release the texture's hardware/streaming resource. boundary
    // (not reversed in this batch; called from hcex_change_render_mode).
    bool UnloadResource();

    // 0x826F6030 / 0x826F6070 -- reversed in txmTEXTURE__GetSizeX.cpp / txmTEXTURE__GetSizeY.cpp.
    // Logical width/height: follows the pSplitScreenTex[0] chain to the leaf when split-screen
    // scaling is active (txmManager state bit 0x8000), else this texture's own sizeX/sizeY.
    int GetSizeX() const;
    int GetSizeY() const;

    // vtable -- push the 4-channel swizzle-source selection down to the derived (D3D) texture.
    // boundary (external to this batch; called from UpdateTextureSwizzle).
    void SetSwizzle(TXM_TEX_SWIZZLE_SRC r, TXM_TEX_SWIZZLE_SRC g, TXM_TEX_SWIZZLE_SRC b,
                             TXM_TEX_SWIZZLE_SRC a);

    // Protected helper (IAA) invoked by the derived txmD3D_TEX::Resize: PrepareSizeFormat
    // recomputes mip count / format packing from the just-set sizeX/sizeY. boundary (external
    // to this batch).
    void PrepareSizeFormat();

    // 0x826F61E4 (?RecreateSplitScreen@txmTEXTURE@@IAAXXZ, protected) -- reversed in
    // txmTEXTURE__RecreateSplitScreen.cpp. When this is a split-screen master (state bit 0x8) and
    // the current video mode is split (vmState & 0xC00), re-points each pSplitScreenTex[] sibling
    // at its sub-rectangle of pData and Resize()s it to the per-viewport dimensions (sizeX/divX,
    // sizeY/divY where divX/divY are 2 for the horizontal/vertical split bits 0x800/0x400).
    void RecreateSplitScreen();

    // vtable -- return the pixel-data pointer for split-screen sub-viewport `idx` (0/1): an
    // offset into this master texture's pData. boundary (external to this batch; called from
    // RecreateSplitScreen).
    unsigned char *GetSplitData(int idx);

    // vtable (UAA) -- resize the (render-target) surface to w x h. Overridden by txmD3D_TEX::Resize
    // (reversed in txmD3D_TEX__Resize.cpp). Declared on the base so RecreateSplitScreen can invoke
    // it through the pSplitScreenTex[] pointers via virtual dispatch.
    unsigned int Resize(unsigned int w, unsigned int h);

    // Overload of SetSwizzle taking the pre-packed 4-channel swizzle-source word (as stored in
    // txmTEXTURE::swizzle), used by d3dDRIVER_INTERFACE::EnsureTexture's per-stage swizzle
    // override. boundary (external to this batch).
    void SetSwizzle(unsigned int packedSwizzle);

    // Virtual (UAA) base: record the gamma-enable flag in the texture's state. The derived
    // txmD3D_TEX override calls this first, then updates the hardware fetch constant. boundary
    // (external to this batch; called from txmD3D_TEX::EnableGamma).
    void EnableGamma(int enable);

    // 0x823EF360 -- reversed in txmTEXTURE__Release.cpp.
    unsigned int Release();

    // 0x826F60B4 -- reversed in txmTEXTURE__GetRTSizeX.c. Render-target width: rtWidth if set,
    // else (when this is a split-screen viewport's texture) the primary viewport's GetSizeX(),
    // else the tag-authored sizeX.
    int GetRTSizeX() const;

    // 0x826F60FC -- reversed in txmTEXTURE__GetRTSizeY.c. Mirror of GetRTSizeX for height.
    int GetRTSizeY() const;

    // 0x826F6D84 -- reversed in txmTEXTURE__SetDesc.c. Installs `_desc`, derives the sRGB state
    // bit, re-derives the hardware swizzle, and folds a couple of usage-bit-driven state/address
    // overrides (state bits 0x4000/0x800 for the 0x18/0x10000 usage-bit groups; forces WRAP
    // addressing when a cube/volume-map usage bit in 0x6000 is set).
    void SetDesc(const ENGINE_TEX_DESC &_desc);

    // 0x826F57A4 -- reversed in txmTEXTURE__UpdateTextureSwizzle.c. Derives this->swizzle (and
    // propagates it to both split-screen sub-textures via SetSwizzle) from desc.usage.maps' four
    // "extra map" bits (0x200/0x400/0x800/0x1000), picking DXN-aware defaults per
    // vidDriver->sdrPlatform.dxn when 1 or 2 of those bits are set. No-op unless at least one of
    // usage.maps' bits 0x1E00 is set.
    void UpdateTextureSwizzle();

    // 0x826F59C8 (?MarkAsUsedInRender@txmTEXTURE@@QAAXXZ, public) -- reversed in
    // txmTEXTURE__MarkAsUsedInRender.cpp. Stamps frameNumUpd with txmManager->currFrame, flagging
    // this texture as touched this frame for the streaming manager's used-vs-idle bookkeeping.
    void MarkAsUsedInRender();

    // 0x826F6488 (?Set@txmTEXTURE@@UAAHPAUpctPICTURE@@H@Z, virtual UAA) -- reversed in
    // txmTEXTURE__Set.cpp. Assign a source picture; isDelayed stashes it in pPictSrc (state bit
    // 0x2000000) for a deferred upload, otherwise (re)creates the surface and uploads immediately.
    int Set(pctPICTURE *pPict, int isDelayed);

    // 0x823EF400 (?GetSplitScreenTexture@txmTEXTURE@@QAAPAV1@H@Z, public) -- reversed in
    // txmTEXTURE__GetSplitScreenTexture.cpp. Returns pSplitScreenTex[idx]; idx must be 0 or 1.
    txmTEXTURE *GetSplitScreenTexture(unsigned int idx);

    // 0x826FDBCC (?Create@txmTEXTURE@@UAAHHHHHHH@Z, virtual UAA) -- reversed in
    // txmTEXTURE__Create.cpp. (Re)establish the texture's geometry/format: no-op fast path when
    // already created (state bit 0x400000) with matching params, else store sx/sy/sz/mips/format,
    // apply the driver size request, clamp mip count for block-compressed formats (deepest mip's
    // min dimension kept >= 4 texels), allocate the driver surface (CreateDrv) and duplicate the
    // split-screen siblings. Returns 1 on success, 0 if CreateDrv fails.
    int Create(int sx, int sy, int nMipMap, int format, int nFaces, int sz);

    // LoadDrv uploads the picture's pixels to the driver. Overridden by txmD3D_TEX::LoadDrv
    // (reversed in txmD3D_TEX__LoadDrv.cpp). boundary at this level (base is abstract).
    int LoadDrv(pctPICTURE *pPict);

    // Helpers invoked by Create. boundary (external to this batch):
    //  - SetRTFormat: derive the render-target hardware format from a pct format + multisample;
    //  - RequestSize (vtable): let the driver adjust requested w/h/d (padding/pow2/limits) in place;
    //  - CreateDrv (vtable): allocate the underlying driver surface; returns nonzero on success;
    //  - DuplicateSplitScreen: (re)create the pSplitScreenTex[] siblings for the new geometry.
    void SetRTFormat(unsigned int pctFormatNmb, TXM_MULTISAMPLE ms, int a3, int a4);
    void RequestSize(int *w, int *h, int *d);
    int  CreateDrv();
    void DuplicateSplitScreen();

    // 0x82655F68 (?Swap@txmTEXTURE@@MAAXXZ, protected in the original -- declared public here so
    // strmRES_TEX_MANAGER::NecessaryStreamTexture can call it, matching the binary's direct
    // vtable dispatch, which is not access-control-checked) -- swap the just-finished-streaming
    // surface into the texture's live slot. boundary, not decompiled here.
    void Swap();
} txmTEXTURE;
