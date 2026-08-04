#pragma once
// ws-engine obj: a single triangle face — three vertex indices into the object's vertex
// arrays. DB-verified layout (types_members objFACE) — size 6.

typedef struct objFACE {
    unsigned __int16 ind[3]; // 0x00 vertex indices of the triangle corners
} objFACE;
