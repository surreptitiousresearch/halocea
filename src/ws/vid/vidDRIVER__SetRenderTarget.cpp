#include "../../headers/vidDRIVER.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

// @0x82668C38 -- ?SetRenderTarget@vidDRIVER@@QAAHPAVtxmTEXTURE@@0V?$apSTATE_T@K@@@Z
// Public convenience wrapper: build a single-color-target render-target stack entry from the
// given color/depth textures and def-use state, then apply it through the virtual
// SetRenderTarget(const vidRT_STACK_ENTRY *) slot. Afterwards latch the current target's pixel
// dimensions (from the color target, else the depth target, else the display mode) and derive the
// texel-size / half-texel shader parameters. Returns 1.
//
// NOTE: the DB mangles the 4th parameter as apSTATE_T<unsigned long> BY VALUE (the decompiler
// mistyped it as a pointer). PopRenderTarget passes the literal 3 for it.
int vidDRIVER::SetRenderTarget(txmTEXTURE *pTexRend, txmTEXTURE *pTexDepth,
                               apSTATE_T<unsigned long> stateDefUse)
{
    vidRT_STACK_ENTRY entry;
    entry.state[0] = stateDefUse;
    entry.state[1].state = 0;
    entry.pTexRendTarg[0] = pTexRend;
    entry.pTexRendTarg[1] = nullptr;
    entry.pTexRendTargZS = pTexDepth;
    entry.sx = 0;
    entry.sy = 0;

    this->SetRenderTarget(&entry);

    txmTEXTURE *colorTarget = this->rendTargCur.pTexRendTarg[0];
    if (colorTarget)
    {
        this->rendTargCur.sx = colorTarget->GetRTSizeX();
        this->rendTargCur.sy = this->rendTargCur.pTexRendTarg[0]->GetRTSizeY();
    }
    else if (this->rendTargCur.pTexRendTargZS)
    {
        this->rendTargCur.sx = this->rendTargCur.pTexRendTargZS->GetRTSizeX();
        this->rendTargCur.sy = this->rendTargCur.pTexRendTargZS->GetRTSizeY();
    }
    else
    {
        vidMODE *pMode = this->pMode;
        this->rendTargCur.sx = pMode->sx;
        this->rendTargCur.sy = pMode->sy;
    }

    // Texel-size and half-texel-offset shader params: [0]=1/sx, [1]=1/sy, [2]=0.5/sx, [3]=0.5/sy.
    // (The decompiler's int->float scratch-store idiom is normalized to plain conversions here.)
    float sx = (float)this->rendTargCur.sx;
    float sy = (float)this->rendTargCur.sy;
    this->rendTargCurParam[0] = 1.0f / sx;
    this->rendTargCurParam[1] = 1.0f / sy;
    this->rendTargCurParam[2] = (1.0f / sx) * 0.5f;
    this->rendTargCurParam[3] = (1.0f / sy) * 0.5f;

    return 1;
}
