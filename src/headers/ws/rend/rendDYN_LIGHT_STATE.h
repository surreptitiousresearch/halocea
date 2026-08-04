#pragma once
// ws-engine rend: per-object dynamic-light selection state.
// DB-verified layout (types_members rendDYN_LIGHT_STATE) — size 24 (0x18).

typedef struct rendDYN_LIGHT_STATE {
    unsigned int dynLightsMask[2];         // 0x00
    unsigned int dynLightsMaskFiltered[2]; // 0x08
    char         nDynLights[2];            // 0x10
    char         nSMLights[2];             // 0x12
    unsigned int lastLgtUpdFrameNmb;       // 0x14

    // Default-construct (zero every field). Body not decompiled here — called by
    // animINST::animINST(). boundary
    rendDYN_LIGHT_STATE();

    // 0x8273E290 — invalidate cached masks when the frame number changed (reversed in
    // rendDYN_LIGHT_STATE__ResetLightsMaskIfNeeded.cpp).
    void ResetLightsMaskIfNeeded(unsigned int frameNmb);
} rendDYN_LIGHT_STATE;
