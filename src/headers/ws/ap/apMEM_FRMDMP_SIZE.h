#pragma once
// ws-engine ap: running/peak byte pair sampled by a frame memory dumper.
// DB-verified layout (types_members apMEM_FRMDMP_SIZE): size@0, sizeMax@4 — size 8.

typedef struct apMEM_FRMDMP_SIZE {
    int size;    // 0x00 current sample
    int sizeMax; // 0x04 peak sample
} apMEM_FRMDMP_SIZE;
