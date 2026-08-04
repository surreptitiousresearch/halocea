#pragma once
// ssl subsystem: saved script-stack cursor snapshot. DB-verified layout
// (types_members sslSTACK_STATE): mainStackSize@0, localsStackSize@4, foreachStackSize@8,
// namedParamsPos@12 — size 16.

typedef struct sslSTACK_STATE {
    int mainStackSize;    // 0x00
    int localsStackSize;  // 0x04
    int foreachStackSize; // 0x08
    int namedParamsPos;   // 0x0C
} sslSTACK_STATE;
