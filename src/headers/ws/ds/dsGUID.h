#pragma once
// ws-engine ds: 128-bit GUID. DB-verified layout (types_members dsGUID): d[4]@0 — size 16.
typedef struct dsGUID {
    unsigned int d[4]; /* 0x00 */
} dsGUID;
