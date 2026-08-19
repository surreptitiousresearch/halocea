/* decal_delete @0x83740798 — unlinks decal `decal_index` from its per-(layer,cluster) linked list (or the
 * "disconnected" list when cluster_index == -1) and frees its datum slot. Decal record layout established
 * by decals_delete_permanent_from_cluster.c: 56-byte stride, chained through decal+52 ("next"); this
 * function additionally uses decal+48 ("prev"), decal+4 (cluster_index, short), decal+6 (layer_index,
 * short) — all confirmed via disasm, no struct header exists yet for the full decal datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

/* DEVIATION (2026-08-12, #134): decal_index is `int`, a full datum handle, NOT the `uint16_t` this
 * was declared with. `mr r4, r3 # index` @0x837407A0 copies the parameter WHOLE into datum_delete's
 * index argument and r4 is never rewritten before `b datum_delete` @0x8374084C. datum_delete splits
 * that argument at 0x836FB4C4 (`srawi r10, r4, 0x10` -> `cmpwi cr6, r10, 0` -> `beq` accept), so a
 * zero-extended uint16_t makes the salt always 0 and the generation check never runs. The
 * `clrlwi r11, r3, 16` @0x8374079C two instructions earlier is DATA_ARRAY_ELEMENT's own truncation
 * for the local subscript (data_array.h), not an ABI width. */
void decal_delete(int decal_index)
{
    decal_datum *decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_index);
    int prev_decal_index = decal->prev_decal_index;
    int next_decal_index = decal->next_decal_index;

    if ( next_decal_index != -1 )
    {
        decal_datum *next_decal = DATUM_GET(global_decal_data, decal_datum, next_decal_index);
        next_decal->prev_decal_index = prev_decal_index;
    }

    if ( prev_decal_index == -1 )
    {
        int16_t cluster_index = decal->cluster_index;
        if ( cluster_index == -1 )
            decal_globals->first_disconnected_decal_index = next_decal_index;
        else
            decal_globals->first_decal_indices[decal->layer][cluster_index] = next_decal_index;
    }
    else
    {
        decal_datum *prev_decal = DATUM_GET(global_decal_data, decal_datum, prev_decal_index);
        prev_decal->next_decal_index = next_decal_index;
    }

    datum_delete(global_decal_data, decal_index);
}
