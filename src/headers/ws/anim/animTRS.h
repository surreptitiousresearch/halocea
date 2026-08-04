#pragma once
#include "../m3d/m3dV.h"
#include "../m3d/m3dQUAT.h"
#include "../m3d/m3dMATR.h"
#include "../ap/apSTATE_T.h"
// ws-engine anim: decomposed transform (translate/rotate/scale + visibility). DB-verified layout
// (types_members animTRS) — size 45: visibility@0 (float), trans@4 (m3dV), scale@16 (m3dV),
// rot@28 (m3dQUAT), type@44 (apSTATE_T<unsigned char>). SetMatr/GetMatr/Blend are boundaries.

typedef struct animTRS {
    float                     visibility; // 0x00
    m3dV                      trans;      // 0x04
    m3dV                      scale;      // 0x10
    m3dQUAT                   rot;        // 0x1C
    apSTATE_T<unsigned char>  type;       // 0x2C

    // Decompose `matr` into this TRS; `mask` selects which components. boundary
    void SetMatr(const m3dMATR *matr, unsigned int mask);
    // Compose this TRS back into `matr`. boundary
    void GetMatr(m3dMATR *matr);
    // Blend this <- lerp(this, other, t). boundary
    void Blend(const animTRS *other, float t);
} animTRS;
