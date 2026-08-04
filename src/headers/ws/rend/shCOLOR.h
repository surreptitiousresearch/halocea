#pragma once
#include "shSH.h"
// ws-engine rend: RGB triple of spherical-harmonics coefficient sets.
// DB-verified layout (types_members shCOLOR): shR@0, shG@36, shB@72 — size 108.

struct m3dV;         // ../m3d/m3dV.h — direction (pointer only)  boundary
struct m3dCOLOR;     // ../m3d/m3dCOLOR.h — RGBA (pointer only)   boundary
struct shSHD_COEFFS; // shSHD_COEFFS.h — shader coefficient block (pointer only)  boundary

typedef struct shCOLOR {
    shSH shR; // 0x00
    shSH shG; // 0x24
    shSH shB; // 0x48

    // rend SH-lighting accumulation, used by rendLIGHT_SET::UpdateStatic.
    // 0x83095698 — project a directional light of `color` from unit direction `dir` onto the SH
    // basis and accumulate. REVERSED: src/ws/rend/shCOLOR__AddLight.cpp.
    void AddLight(const m3dV *dir, const m3dCOLOR *color);
    // 0x8309627C — directional overload: normalise (lightPos-pos), scale `color` by `intensity`,
    // then forward to AddLight(dir,color). REVERSED: src/ws/rend/shCOLOR__AddLight_pos.cpp.
    void AddLight(const m3dV *pos, const m3dV *lightPos, float intensity, const m3dCOLOR *color);
    // 0x83095A00 — add a flat ambient term `intensity`*`color` to the DC (band-0) coefficients.
    // REVERSED: src/ws/rend/shCOLOR__AddLightAmb.cpp.
    void AddLightAmb(float intensity, const m3dCOLOR *color);
    // 0x83095AB4 — bake the SH coefficients into the packed shader-constant block.
    // REVERSED: src/ws/rend/shCOLOR__CalcShaderConstants.cpp.
    void CalcShaderConstants(shSHD_COEFFS *outCoeffs);
} shCOLOR;
