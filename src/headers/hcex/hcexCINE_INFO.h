#pragma once
#include "../ws/ap/apSTATE_T.h"
struct gsANITEC_ACTOR; // ws-engine gs: cinematic actor being animated (pointer only) — boundary, 444 bytes, out of scope

// HCEX bridge: one entry in the cinematics-in-progress list — pairs a cinematic actor with the
// per-actor "mask" state snapshot used to restore its animation-tec state afterward.
// DB-verified layout (types_members hcexCINE_INFO): actor@0 (gsANITEC_ACTOR*),
// mask@4 (apSTATE_T<unsigned char>) — size 5 (padded to 8 in the containing dsVECTOR<...,8>).
typedef struct hcexCINE_INFO {
    gsANITEC_ACTOR      *actor; // 0x00
    apSTATE_T<unsigned char> mask; // 0x04

    // 0x823B2B80 — record the actor and snapshot its current state byte into `mask`.
    hcexCINE_INFO(gsANITEC_ACTOR *actor, const apSTATE_T<unsigned char> &mask);
} hcexCINE_INFO;
