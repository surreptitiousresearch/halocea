#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/WEAK_PTR.h"
#include "../anim/animTRACKER.h"
// ws-engine ent: follow-hierarchy link block embedded by value in entENTITY. DB-verified
// layout (types_members entFOLLOW) — size 112.

struct entENTITY; // entENTITY.h — pointers only here

typedef struct entFOLLOW {
    apSTATE_T<unsigned char> state;            // 0x00
    unsigned char            _pad1[3];         // 0x01 db-verified padding
    entENTITY               *pEntOwner;        // 0x04
    ds::WEAK_PTR<entENTITY>  pChild;           // 0x08
    entENTITY               *pEntBrotherLeft;  // 0x0C
    entENTITY               *pEntBrotherRight; // 0x10
    entENTITY               *pEntParent;       // 0x14
    animTRACKER              instTrkFollow;    // 0x18 (88B)

    // Re-evaluate this actor's follow pose (called when the actor becomes visible again). boundary.
    void UpdateFollow();
} entFOLLOW;
