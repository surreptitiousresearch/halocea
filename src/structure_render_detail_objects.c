/* structure_render_detail_objects @0x83711A30 — draws the current structure BSP's detail objects
 * (grass, small debris, etc.) for the single local player this build supports rendering them for.
 *
 * Only runs in single-local-player mode with a valid render.local_player_index. If the camera has
 * moved to a different detail-object cell since the last call (or a rebuild was explicitly requested via
 * valid bit 1), re-derives the visible cell set: for each of the 9 cells in a 3x3 neighborhood
 * around the camera's own cell, binary-searches the BSP's sorted (cell_x, cell_y, cell_z) cell-index
 * array for the sub-range matching that (x, y) with z within [camera_z-1, camera_z+2), then for every
 * matching entry within the true ±1 cell Z range, appends one detail_object_cell_data per populated
 * layer into detail_object_global_runtime_data's per-layer cell arrays. Finally rebuilds the view's
 * vertex buffer and draws it.
 *
 * DEVIATION: `abs32(...)` in the decompile is not a real call — disasm_range(0x83711DA0, 0x83711DD0)
 * shows the classic branchless `srawi/xor/subf` abs idiom operating on plain registers, so it is
 * reproduced here as inline integer abs rather than an extern call.
 *
 * DEVIATION: the decompiler shows a 104-element `_WORD v42[104]` scratch array ("local variable
 * allocation failed" class corruption from the huge stack frame), but only indices 0-31 are ever read or
 * written (`memset(v42, 0, 64)` = 32 shorts, and every indexed access uses a value in [0, 32) — the loop
 * variable `layer`). Modeled as `unsigned __int16 layer_object_counts[32]`.
 *
 * FAITHFUL: the outer/inner binary-search loop nest perturbs X in the OUTER loop and Y in the INNER loop
 * (confirmed via disasm: the outer loop variable feeds `query_x`, the inner feeds `query_y`) — easy to
 * get backwards from the decompile's terse `v5`/`v6` naming, so named explicitly here as
 * `x_delta`/`y_delta` in that nesting order.
 *
 * See detail_object_bsp_cell_table.h for the cell-index blob's layout (no DB type exists for it; every
 * field was derived from this function's own disassembly). */

#include <stdint.h>
#include <string.h>

#include "headers/detail_object_global_runtime_data.h"
#include "headers/detail_object_bsp_cell_table.h"
#include "headers/structure_bsp.h"
#include "headers/render_globals.h"

extern int16_t local_player_count(void);
extern void rasterizer_detail_objects_begin(void);
extern void rasterizer_detail_objects_rebuild_vertices(const detail_object_view_data *detail_object_view_data);
extern void rasterizer_detail_objects_draw(const detail_object_view_data *detail_object_view_data);
extern void rasterizer_detail_objects_end(void);

extern detail_object_cell_definition *get_lower_bound_cell(detail_object_cell_definition *begin, detail_object_cell_definition *end, detail_object_cell_coord *key);
extern detail_object_cell_definition * get_upper_bound_cell(detail_object_cell_definition *begin, detail_object_cell_definition *end, detail_object_cell_coord *key);
int structure_render_detail_objects(void)
{
    __int16 result = local_player_count();
    if (result != 1 || render.local_player_index == 0xFFFF)
        return result;

    detail_object_bsp_cell_table *table = global_structure_bsp->detail_object_data.count
            ? (detail_object_bsp_cell_table *)global_structure_bsp->detail_object_data.address
            : nullptr;
    struct detail_object_global_runtime_data *runtime_data = detail_object_global_runtime_data;

    __int16 camera_cell_x = (__int16)(render.camera.position.n[0] * 0.125f - 0.5f);
    __int16 camera_cell_y = (__int16)(render.camera.position.n[1] * 0.125f - 0.5f);
    __int16 camera_cell_z = (__int16)(render.camera.position.n[2] * 0.125f - 0.5f);

    if (!table->valid)
        return result;

    rasterizer_detail_objects_begin();

    detail_object_runtime_data *player0 = &runtime_data->local_player_data[0];
    if (camera_cell_x != player0->last_cell_coord.x
            || camera_cell_y != player0->last_cell_coord.y
            || camera_cell_z != player0->last_cell_coord.z
            || !player0->last_cell_coord.initialized
            || (table->valid & 2) != 0)
    {
        unsigned __int16 layer_object_counts[32];
        memset(layer_object_counts, 0, sizeof(layer_object_counts));
        unsigned int layer_mask_union = 0;

        table->valid = 1;
        player0->last_cell_coord.x = camera_cell_x;
        player0->last_cell_coord.y = camera_cell_y;
        player0->last_cell_coord.z = camera_cell_z;
        player0->last_cell_coord.initialized = 1;

        for (__int16 x_delta = -1; x_delta <= 1; x_delta++)
        {
            for (__int16 y_delta = -1; y_delta <= 1; y_delta++)
            {
                __int16 query_x = camera_cell_x - x_delta;
                __int16 query_y = camera_cell_y - y_delta;
                __int16 query_z_lo = camera_cell_z - 1;
                __int16 query_z_hi = query_z_lo + 3;

                detail_object_bsp_cell_index *cell_indices = (detail_object_bsp_cell_index *)table->cells.address;
                int cell_count = table->cells.count;

                /* DEVIATION: hand-inlined copy of get_lower_bound_cell@0x83711890 (zero out-of-line xrefs) collapsed back to a call -- donor args reconstructed from the surrounding loop: begin=cell_indices, end=cell_indices+cell_count, key=(query_x,query_y,query_z_lo); the disassembly-duplicated `remaining/2` (donor's cached `half`) is an artifact of the inlining and folds away with the call. */
                /* Lower bound: first entry >= (query_x, query_y, query_z_lo). */
                detail_object_cell_coord lower_bound_key = { query_x, query_y, query_z_lo };
                detail_object_bsp_cell_index *first = get_lower_bound_cell(cell_indices, cell_indices + cell_count, &lower_bound_key);

                /* DEVIATION: verbatim inlined copy of get_upper_bound_cell@0x83711938 (zero xrefs at release) collapsed to a call; donor's `key` param reconstructed from the host's local query_x/query_y/query_z_hi triple, donor's begin/end reconstructed from cell_indices/cell_indices+cell_count. */
                /* Upper bound: first entry > (query_x, query_y, query_z_hi). */
                detail_object_cell_coord query_key_hi = { query_x, query_y, query_z_hi };
                detail_object_bsp_cell_index *last = get_upper_bound_cell(cell_indices, cell_indices + cell_count, &query_key_hi);

                detail_object_bsp_cell_index *range_last = last - 1;
                if (first->cell_x == query_x && first->cell_y == query_y
                        && range_last->cell_x == query_x && range_last->cell_y == query_y
                        && first < last)
                {
                    detail_object_bsp_cell_index *entry = first;
                    unsigned int match_count = (unsigned int)(last - first);
                    do
                    {
                        int z_diff = camera_cell_z - entry->cell_z;
                        if (z_diff < 0)
                            z_diff = -z_diff;

                        if (z_diff <= 1)
                        {
                            int first_object_index = 0;
                            __int16 vertex_table_index = 0;
                            layer_mask_union |= entry->valid_layers;

                            for (int layer = 0; layer < 32; layer++)
                            {
                                if (((1 << layer) & entry->valid_layers) != 0)
                                {
                                    int slot = layer_object_counts[layer]++;

                                    /* recovered: (char *)runtime_data + 648*layer + 24*slot -> &player0->cells[layer][slot] */
                                    detail_object_cell_data *cell = &player0->cells[layer][slot];
                                    cell->cell_x = entry->cell_x;
                                    cell->cell_y = entry->cell_y;
                                    cell->cell_z = ((unsigned __int16)entry->offset_z * 0.0039215689f)
                                            + (float)entry->cell_z;
                                    cell->first_detail_object_index = entry->start_index
                                            + first_object_index;

                                    int vertex_index = entry->count_index + vertex_table_index;
                                    unsigned __int16 detail_object_count = ((unsigned __int16 *)table->detail_objects_counts.address)[vertex_index];
                                    cell->detail_object_count = detail_object_count;

                                    cell->z_reference_vector = table->detail_object_z_reference_vectors.count
                                            ? &((const real_vector4d *)table->detail_object_z_reference_vectors.address)[vertex_index]
                                            : &runtime_data->default_z_reference_vector;

                                    first_object_index += detail_object_count;
                                    vertex_table_index++;
                                }
                            }
                        }

                        entry++;
                        match_count--;
                    }
                    while (match_count);
                }
            }
        }

        detail_object_view_data *view = &player0->view_data;
        view->layer_count = 0;
        view->layers = player0->layers;

        int layer_slot = 0;
        for (int layer = 0; layer < 32; layer++)
        {
            if (((1 << layer) & layer_mask_union) != 0 && layer_object_counts[layer])
            {
                detail_object_layer_data *layer_data = &view->layers[layer_slot++];
                layer_data->cell_count = layer_object_counts[layer];
                /* recovered: (char *)runtime_data + 648*layer -> player0->cells[layer] */
                layer_data->cells = player0->cells[layer];
                layer_data->collection_definition_index = layer;
                view->layer_count++;
            }
        }

        rasterizer_detail_objects_rebuild_vertices(view);
    }

    rasterizer_detail_objects_draw(&player0->view_data);

    /* DEVIATION: the decompile shows `return rasterizer_detail_objects_end();`, but that function (and
     * its real `_rasterizer_detail_objects_end` implementation) is void — an empty stub. The PPC r3
     * register is left holding whatever it last held (unspecified), so this is not a meaningful return
     * value; reproduced as a plain call followed by returning the earlier `local_player_count()` result. */
    rasterizer_detail_objects_end();
    return result;
}
