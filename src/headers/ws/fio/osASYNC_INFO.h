#pragma once
// ws-engine os: async-I/O completion record, DB-verified layout
// (types_members osASYNC_INFO): size 68.

struct osHANDLE_DUMMY; // boundary — opaque OS wait handle

typedef struct osASYNC_INFO {
    int             isEOF;       // 0x00 request hit end of stream
    osHANDLE_DUMMY *signal;      // 0x04 completion signal handle
    int             isSignaled;  // 0x08 completion latched
    int             mustRetry;   // 0x0C retry requested
    int             holdSignal;  // 0x10 keep the signal handle across retries
    unsigned char   data[48];    // 0x14 platform overlapped/async scratch

    // Tear down a pending async-I/O tracking record: releases the completion signal
    // handle (if held) and clears holdSignal. Defined in src/ws/os/osASYNC_INFO__Term.cpp.
    void Term();
} osASYNC_INFO;
