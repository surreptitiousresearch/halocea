#pragma once
// ws-engine ds: intrusive reference-count word embedded in ref-counted objects.
// DB-verified layout (types_members dsREF_COUNT): refCount@0 — size 4.

typedef struct dsREF_COUNT {
    int refCount; // 0x00
} dsREF_COUNT;
