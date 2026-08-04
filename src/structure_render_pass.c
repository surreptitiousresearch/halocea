#include <stdint.h>
#include <stddef.h>   /* NULL */
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/global_tag_instances.h"
#include "headers/shader.h"
#include "headers/bitmap_data.h"
#include "headers/vertex_buffer.h"
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"
#include "headers/render_lighting.h"

extern uint8_t breakable_surface_extant(int16_t breakable_surface_index);
extern uint8_t shader_type_is_transparent(int16_t shader_type);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);

/* DEVIATION: the DB's stored callback type for draw_transparent_triangles is the 11-arg impl
 * signature (matches rasterizer_environment_transparent_geometry_submit's own prototype), but the
 * call site below explicitly casts the pointer to an 8-arg signature and supplies only 8 values —
 * plane/offset/render_lighting/geometry_flags are left as whatever garbage sits in this function's
 * stack frame at those offsets. Typed here to match what is actually invoked. */
void structure_render_pass(
        int    *surface_indices,
        __int16 surface_count,
        int     dynamic_triangles_index,
        void (__fastcall *begin_lightmap)(bitmap_data *),
        void (__fastcall *draw_triangles)(const shader *, __int16, int, int, int, const vertex_buffer *),
        void (__fastcall *end_lightmap)(void),
        void (__fastcall *draw_transparent_triangles)(const shader *, __int16, bitmap_data *, int, int, int,
                                                       const vertex_buffer *, const real_point3d *))
{
    structure_bsp *bsp                = global_structure_bsp;
    int           *surface_cursor     = surface_indices;
    int           *surface_end        = &surface_indices[surface_count];
    int            first_triangle_index = 0;

    if (bsp->lightmaps.count <= 0)
        return;

    structure_lightmap *lightmaps = (structure_lightmap *)bsp->lightmaps.address;

    for (__int16 lightmap_index = 0; lightmap_index < bsp->lightmaps.count; lightmap_index++)
    {
        if (surface_cursor >= surface_end)
            break;

        structure_lightmap *page = &lightmaps[lightmap_index];
        /* recovered: page->material_count/page->materials -> tag_block members materials.count/.address */
        structure_material *page_materials = (structure_material *)page->materials.address;

        /* skip this lightmap page entirely if none of the remaining surface indices fall within
         * the surface range covered by its materials (checked against the last material's range) */
        structure_material *last_material = &page_materials[page->materials.count - 1];
        if (*surface_cursor >= last_material->first_surface_index + last_material->surface_count)
            continue;

        int bitmap_group_index = bsp->lightmap_group.index;
        bitmap_data *bitmap = (bitmap_group_index == -1)
                ? NULL
                : bitmap_group_try_and_get_bitmap(bitmap_group_index, page->bitmap_index);

        if (begin_lightmap)
            begin_lightmap(bitmap);

        for (int material_index = 0; material_index < page->materials.count; material_index++)
        {
            if (surface_cursor >= surface_end)
                break;

            structure_material *material    = &page_materials[material_index];
            int                 surface_end_idx = material->first_surface_index + material->surface_count;

            if (*surface_cursor < surface_end_idx)
            {
                int *chunk_start = surface_cursor;
                /* renamed local `shader` -> `surface_shader`: it shadowed the `shader` type,
                 * breaking the `(const shader **)` cast in its own initializer. */
                const shader *surface_shader = TAG_GET(const shader, material->shader.index);

                do
                    ++surface_cursor;
                while (surface_cursor < surface_end && *surface_cursor < surface_end_idx);

                __int16 chunk_count = (__int16)(surface_cursor - chunk_start);

                if (breakable_surface_extant(material->breakable_surface_index))
                {
                    if (shader_type_is_transparent(surface_shader->base.type))
                    {
                        if (draw_transparent_triangles)
                            draw_transparent_triangles(
                                    surface_shader,
                                    material->permutation_index,
                                    bitmap,
                                    dynamic_triangles_index,
                                    first_triangle_index,
                                    chunk_count,
                                    &material->vertices,
                                    &material->centroid);
                    }
                    else if (draw_triangles)
                    {
                        draw_triangles(
                                surface_shader,
                                material->permutation_index,
                                dynamic_triangles_index,
                                first_triangle_index,
                                chunk_count,
                                &material->vertices);
                    }
                }

                first_triangle_index += chunk_count;
            }
        }

        if (end_lightmap)
            end_lightmap();
    }
}
