#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/scenario.h"
#include "headers/structure_bsp.h"
#include "headers/cache_file_tag_instance.h"
#include "headers/global_tag_instances.h"
#include "headers/detail_object_global_runtime_data.h"
#include "headers/detail_object_collection_definition.h"
#include "headers/detail_object.h"
#include "headers/detail_object_vertex.h"
#include "headers/blam_data_globals.h"

extern int16_t main_get_window_count(void);

extern int   hcex_stat_vbuf_lock(void *static_vbuf, int ofs, int size, void **buf_ptr);
extern void  hcex_stat_vbuf_unlock(void *static_vbuf);

extern void detail_object_build_vertices(const detail_object_collection_definition *collection_definition, const detail_object *detail_objects, detail_object_vertex *detail_object_vertices, int detail_object_count, detail_object_cell_data *cell);

void _rasterizer_detail_objects_rebuild_vertices(const detail_object_view_data *detail_object_view_data)
{
    if (!rasterizer_debug_options.draw_detail_objects || main_get_window_count() > 1)
        return;

    detail_object_vertex *vertex_buf = NULL;
    local_d3d_vertex_buffer_ofs = hcex_stat_vbuf_lock(
            local_d3d_vertex_buffer_0, 0, 491520, (void **)&vertex_buf);
    if (vertex_buf)
    {
        scenario *scen        = global_scenario;
        int      *detail_data = (int *)(global_structure_bsp->detail_object_data.count
                                        ? global_structure_bsp->detail_object_data.address
                                        : NULL);
        int  vertex_index        = 0;
        /* offset 16 in the first detail_object_data block element holds the base address
         * of the raw detail_object array, stored as int on 32-bit PPC */
        int  detail_objects_base = detail_data[4];
        int  total_drawn         = 0;
        char overflow            = 0;

        if (detail_object_view_data->layer_count > 0)
        {
            short layer_index = 0;
            do
            {
                detail_object_layer_data *layer = &detail_object_view_data->layers[layer_index];

                /* palette entry datum → tag instance → collection definition pointer */
                const detail_object_collection_definition *coll_def =
                    TAG_GET(const detail_object_collection_definition, *((int *)scen->detail_object_collection_palette.address + 12 * layer->collection_definition_index + 3));

                if (layer->cell_count > 0)
                {
                    short cell_index = 0;
                    do
                    {
                        detail_object_cell_data *cell  = &layer->cells[cell_index];
                        int                      count = cell->detail_object_count;
                        if (count > 4096 - total_drawn)
                            count = 4096 - total_drawn;

                        detail_object_build_vertices(
                                coll_def,
                                (const detail_object *)(6 * cell->first_detail_object_index
                                                        + detail_objects_base),
                                &vertex_buf[vertex_index],
                                count,
                                cell);

                        /* DEVIATION: decompiler uses v14 here; same r7 register as cell (v12) pre-call */
                        int built = cell->detail_object_count;
                        cell->internal__first_vertex_index = vertex_index;
                        vertex_index += 6 * built;
                        if (built > count)
                        {
                            cell->detail_object_count = 2 * count;
                            if (!overflow)
                                overflow = 1;
                        }
                        total_drawn += count;

                        cell_index = (int16_t)(cell_index + 1);
                    }
                    while (cell_index < layer->cell_count);
                }

                layer_index = (int16_t)(layer_index + 1);
            }
            while (layer_index < detail_object_view_data->layer_count);
        }
    }

    hcex_stat_vbuf_unlock(local_d3d_vertex_buffer_0);
}
