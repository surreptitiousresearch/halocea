#pragma once
/* packed_parameter — the wire-packed form of the tunable parameter set, produced for transmission by
 * pack_parameter_set_for_transmission. DB: struct with a single anonymous-union member ___u0
 * ($ in identifiers is an MSVC extension). */

/* DB anonymous union _91340FA2F5EECBB2E05E253BA4F0F352 (4 bytes) — packed_parameter member ___u0. */
typedef union _91340FA2F5EECBB2E05E253BA4F0F352
{
    float real_param;    /* 0x00 */
    int   integer_param; /* 0x00 */
} _91340FA2F5EECBB2E05E253BA4F0F352;

typedef struct packed_parameter
{
    /* DB names this union member ___u0; anonymized so consumers reach
     * real_param/integer_param directly (no consumer uses the ___u0 name).
     * Layout unchanged (4 bytes). */
    union {
        float real_param;    /* 0x00 */
        int   integer_param; /* 0x00 */
    };
} packed_parameter; /* 4 bytes */
