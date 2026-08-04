#pragma once
// ws-engine fio: on-disk chunk header read/written around chunked file data.
// DB-verified layout (types_members fioCHUNK / fioCHUNK_SAVE) — size 16.

// fioCHUNK_SAVE — the persisted portion: id@0 (unsigned __int16), offsetEnd@4 (int) — size 8.
typedef struct fioCHUNK_SAVE {
    unsigned __int16 id;         // 0x00 chunk id
    int              offsetEnd;  // 0x04 end offset in stream
} fioCHUNK_SAVE;

// fioCHUNK — fioCHUNK_SAVE base @0, offsetStart@8, descrID@12 — size 16.
typedef struct fioCHUNK : fioCHUNK_SAVE {
    int              offsetStart; // 0x08 start offset in stream
    int              descrID;     // 0x0C descriptor id
} fioCHUNK;
