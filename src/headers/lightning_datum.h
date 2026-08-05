#pragma once
#include <stdint.h>
/* lightning_datum — one entry in the lightning_globals.lightning_data array: a live lightning effect
 * instance bound to a loaded lightning tag definition. Layout from the database (8 bytes). */

typedef struct lightning_datum
{
    int16_t          identifier;       /* 0x0 */
    uint16_t flags;            /* 0x2 */
    int              definition_index; /* 0x4 — tag index of the lightning_definition */
} lightning_datum;
