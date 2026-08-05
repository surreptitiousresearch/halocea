#pragma once
#include <stdint.h>
/* widget_datum — one live object widget (flag/antenna/glow/lightning/vector) attached to an object
   (12 bytes; DB-verified). */

typedef struct widget_datum
{
    int16_t identifier;        /* 0x00 — data_array datum identifier */
    int16_t type;              /* 0x02 — index into widget_type_definitions */
    int     type_datum_index;  /* 0x04 — the type-specific datum (or -1) */
    int     next_widget_index; /* 0x08 — the object's widget list link */
} widget_datum;                /* 12 bytes */
