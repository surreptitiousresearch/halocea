#pragma once
// ws-engine msg: fixed header word prefixing a queued message payload.
// DB-verified layout (types_members msgDATA): sign@0, sizeStruct@2 — a 1-byte gap at offset 1.

typedef struct msgDATA {
    char  sign;       // 0x00 payload tag / signature byte
    char  _pad1;      // 0x01 (DB gap)
    short sizeStruct; // 0x02 payload struct size
} msgDATA;
