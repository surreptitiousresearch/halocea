#pragma once
/* lightning_datum — one entry in the lightning_globals.lightning_data array: a live lightning effect
 * instance bound to a loaded lightning tag definition. Layout from the database (8 bytes). */

typedef struct lightning_datum
{
    __int16          identifier;       /* 0x0 */
    unsigned __int16 flags;            /* 0x2 */
    int              definition_index; /* 0x4 — tag index of the lightning_definition */
} lightning_datum;
