/* ?Create@txmTEXTURE@@UAAHHHHHHH@Z @0x826FDBC0 */
// ?Create@txmTEXTURE@@UAAHHHHHHH@Z  (0x826FDBCC)
// (Re)create the texture's geometry and driver surface. Returns 1 on success, 0 if the driver
// surface allocation fails. The a8..a24 "parameters" the decompiler printed are stack spill
// slots (RequestSize scratch), not real arguments -- the mangled signature UAAHHHHHHH is
// int Create(int,int,int,int,int,int).
// Source: D:\Projects\code\common\src.sys ... (txm subsystem)

#include "../../headers/ws/txm/txmMANAGER.h"  /* txmTEXTURE + txmManager */
#include "../../headers/pct_boundary.h"       /* pctIsCompressed */

// txmTEXTURE::state bits.
enum {
    TXM_STATE_RT_USAGE = 0x300,     // bits 8/9: render-target usage -> needs a hw RT format
    TXM_STATE_CREATED  = 0x400000,  // bit 22: surface already created
};
// txmManager->state: split-screen scaling active.
enum { TXM_MGR_STATE_SPLITSCREEN = 0x8000 };

// Minimum texels per dimension at the deepest mip for a block-compressed (DXT) format.
enum { DXT_MIN_MIP_TEXELS = 4 };

int txmTEXTURE::Create(int sx, int sy, int _nMipMap, int pctFormatNmb, int faces, int sz)
{
    // Fast path: already created -- report whether the requested geometry/format matches.
    if (this->state.state & TXM_STATE_CREATED) {
        return (this->sizeX == sx || !sx)
            && (this->sizeY == sy || !sy)
            && this->nMipMap == _nMipMap
            && this->sizeZ == sz
            && this->nFaces == faces
            && ((unsigned int)pctFormatNmb == (unsigned int)-1 || this->fmtNmb == pctFormatNmb);
    }

    this->nFaces  = faces;
    this->nMipMap = _nMipMap;
    this->fmtNmb  = pctFormatNmb;

    if (this->state.state & TXM_STATE_RT_USAGE)
        this->SetRTFormat(pctFormatNmb, this->rtMultisample, 0, 0);

    int reqW = sx, reqH = sy, reqD = sz;
    if (sx > 0 && sy > 0 && sz > 0) {
        // Driver may pad / adjust the requested dimensions in place.
        this->RequestSize(&reqW, &reqH, &reqD);
    }
    this->sizeX = (short)reqW;
    this->sizeY = (short)reqH;
    this->sizeZ = (short)reqD;
    this->PrepareSizeFormat();

    if (pctIsCompressed(pctFormatNmb)) {
        // Block-compressed formats require the deepest mip's smaller dimension to stay >= 4
        // texels; clamp the mip count accordingly. Dimensions follow the split-screen leaf when
        // split-screen scaling is active.
        txmTEXTURE *split0 = this->pSplitScreenTex[0];
        bool useSplit = split0 && (txmManager->state.state & TXM_MGR_STATE_SPLITSCREEN);
        int curW = useSplit ? split0->GetSizeX() : this->sizeX;
        int curH = useSplit ? split0->GetSizeY() : this->sizeY;
        int minDim = (curW <= curH) ? curW : curH;

        int mip = this->nMipMap - 1;
        // DEVIATION: the decompiler lost the induction registers (v39/v40 undefined) inside this
        // refinement loop; reconstructed by intent from the DXT block-size constraint -- shrink
        // the mip count until the deepest level's min dimension is at least DXT_MIN_MIP_TEXELS.
        while (mip > 0 && (minDim >> mip) < DXT_MIN_MIP_TEXELS)
            --mip;
        this->nMipMap = mip + 1;
    }

    if (!this->CreateDrv())
        return 0;

    this->DuplicateSplitScreen();
    this->state.state |= TXM_STATE_CREATED;
    return 1;
}
