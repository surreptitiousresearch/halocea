/* scenario_debug_to_file @0x83703ED8 — dump a one-line scenario/BSP summary and a per-player status line
 * to `stream`: for each player, print its object index and either " dead\n" (no controlled unit/object)
 * or its position, containing-cluster/leaf, and the string. Debug-only diagnostic dump.
 *
 * DEVIATION: the decompiler renders the second per-player fprintf's format-string argument as
 * `HIDWORD(COERCE_UNSIGNED_INT64(*(float*)(v6+160)))` — nonsense produced by mis-tracking a loop-invariant
 * register (r29) that disasm shows is simply reloaded with the constant format-string address
 * `" at (%.2f,%.2f,%.2f) (leaf#%d,cluster#%d)\n"` at the top of the loop body. Reconstructed as that
 * literal format string with its five real arguments (also confirmed via disasm register tracing, since
 * the decompiler's own float/int arg list was equally garbled by the same misattribution).
 *
 * The two %d fields print the raw bytes of `_object_datum.location` (RAW (irreducible): opaque in
 * object_datum.h, no DB layout) reinterpreted as a leading 4-byte int and a trailing 2-byte int16 — reproduced via that
 * same byte-array field rather than widening the header's guess, since this is the only place in the
 * reconstructed source that reads it. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_structure_bsp_reference.h"
#include "headers/breakable_surface_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

typedef struct _iobuf _iobuf;

extern int fprintf(_iobuf *stream, const char *format, ...);
extern char *tag_get_name(int16_t tag_index);


extern int global_structure_bsp_tag_index_get(void);
void scenario_debug_to_file(_iobuf *stream)
{
    if ( global_scenario_index == -1 )
    {
        fprintf(stream, "<no scenario loaded>\n");
        return;
    }

    /* DEVIATION: inlined copy of global_structure_bsp_tag_index_get@0x83703450 (disasm-confirmed identical fetch: *((_DWORD*)structure_bsp_references.address + 8*global_structure_bsp_index + 7)); collapsed to a call. */
    int bsp_tag_index = global_structure_bsp_tag_index_get();
    const char *bsp_name = tag_get_name(bsp_tag_index);
    const char *scenario_name = tag_get_name(global_scenario_index);
    fprintf(stream, "\"%s\" bsp \"%s\" (#%d)\n", scenario_name, bsp_name, global_structure_bsp_index);

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player;
          player = data_iterator_next(&iterator) )
    {
        fprintf(stream, "player 0x%08x", iterator.index);

        if ( player->unit_index == -1 )
        {
            fprintf(stream, " dead\n");
        }
        else
        {
            object_datum *unit = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);

            /* DEVIATION: object.location is now a typed `location` struct (location.h), not a
             * raw byte array — read the leaf/cluster fields directly (same widths/offsets). */
            int leaf_id = unit->object.location.leaf_index;
            __int16 cluster_id = unit->object.location.cluster_index;

            fprintf(stream, " at (%.2f,%.2f,%.2f) (leaf#%d,cluster#%d)\n",
                unit->object.bounding_sphere_center.n[0],
                unit->object.bounding_sphere_center.n[1],
                unit->object.bounding_sphere_center.n[2],
                leaf_id, cluster_id);
        }
    }
}
