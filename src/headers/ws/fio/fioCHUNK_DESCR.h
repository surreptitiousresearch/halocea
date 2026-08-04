#pragma once
// ws-engine fio: one chunk record in a chunked stream — DB-verified layout
// (types_members fioCHUNK_DESCR): offsetStart@0, offsetEnd@4 — size 8.

typedef struct fioCHUNK_DESCR {
    int offsetStart; // 0x00 file offset of the chunk's size field
    int offsetEnd;   // 0x04 file offset one past the chunk's payload
} fioCHUNK_DESCR;
