#pragma once
#include <stdint.h>
/* hs_global_datum — one element of hs_global_data (8-byte stride). No DB type; layout is
   call-site-determined (hs_global_reconcile_read/write, hs_add_global).
   DB models the value as a plain int (hs_global_runtime); the typed union below is kept for
   readability. Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */

typedef struct hs_global_datum
{
    int16_t identifier;       /* 0x00 — data_array datum identifier */
    int16_t pad;              /* 0x02 */
    union                     /* 0x04 — value, viewed per hs type */
    {
        unsigned char boolean;
        float         real;
        int16_t       short_value;
        int           long_value;
    } value;
} hs_global_datum;            /* 8 bytes */
