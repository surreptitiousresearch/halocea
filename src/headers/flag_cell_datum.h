#pragma once
/* flag_cell_datum — one cloth-mesh cell's tesselation level (2 bytes, DB layout). */

typedef struct flag_cell_datum
{
    __int16 tesselation;  /* 0x0 */
} flag_cell_datum;         /* 2 bytes */
