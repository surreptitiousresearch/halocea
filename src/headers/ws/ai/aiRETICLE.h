#pragma once
// ws-engine ai: reticle-reaction parameter block embedded in bhp behaviour-parameter records.
// DB-verified layout (types_members aiRETICLE): shouldReactToReticle@0, timerSideStep@4,
// distAfraid@8 — size 12.

struct aiRETICLE {
    bool  shouldReactToReticle; // 0x00
    // 3 bytes padding to 0x04
    float timerSideStep;        // 0x04
    float distAfraid;           // 0x08
};
