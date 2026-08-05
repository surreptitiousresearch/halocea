#pragma once
#include <stdint.h>
/* byte_swap_definition — describes how to byte-swap one structure type during endian-safe
 * serialization: element size, the per-field swap-code array, and a verification signature. */

typedef struct byte_swap_definition
{
    char             *name;      /* 0x00 */
    int               size;      /* 0x04 */
    int              *codes;     /* 0x08 */
    unsigned int      signature; /* 0x0C */
    uint8_t   verified;  /* 0x10 */
    uint8_t   pad[3];    /* 0x11 */
} byte_swap_definition; /* 20 bytes */

/* Per-field swap opcodes in byte_swap_definition::codes.
 * DB anonymous enum _0539D0086D0797A5CD8112F55C13D5EA. */
enum byte_swap_code
{
    _extern_bs_definition = -102, /* recurse into a different byte_swap_definition */
    _end_bs_array         = -101, /* end of one element */
    _begin_bs_array       = -100, /* recurse into a nested array, same definition */
    _8byte                = -8,   /* reverse an 8-byte field */
    _4byte                = -4,   /* reverse a 4-byte field */
    _2byte                = -2,   /* reverse a 2-byte field */
    _1byte                = 1,    /* single byte / positive = skip N bytes */
};

/* 'bysw' — verifies a byte_swap_definition code stream. */
#define BYTE_SWAP_DEFINITION_SIGNATURE 0x62797377u /* 'bysw' */
