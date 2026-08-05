/* rasterizer_transparente_geometry_group_draw_zbuffer @0x83812AA8 — z-only draw pass for one queued
 * transparent geometry group: unless the group is flagged non-skinned (geometry_flags bit1), uploads its
 * node-matrix palette (falling back to a single identity matrix if it has none) and, if flagged as
 * needing remapped node parts (bit8), sets those up; applies model lighting if present. For first-person
 * weapon-effect groups (bit7 set, effect.type == 1), narrows the frustum's near/far z to the first-person
 * range for the draw, restoring it to (0,0) afterward. Binds the shared z-only vertex declaration/shader
 * and delegates the actual draw to rasterizer_transparent_geometry_group_draw_internal. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/render_skinning.h"
#include "headers/rasterizer_globals.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/render_model_effect_type.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/blam_data_globals.h"


extern void rasterizer_set_model_skinning(const render_skinning *skinning, uint8_t uploadconstants);
extern void _rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table, real_matrix4x3 *node_matrices);
extern void rasterizer_set_model_lighting(const render_lighting *lighting);
extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);

void rasterizer_transparente_geometry_group_draw_zbuffer(const transparent_geometry_group *group)
{
    if ( !(group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) )
    {
        render_skinning skinning;
        if ( group->node_matrices && group->node_matrix_count )
        {
            skinning.node_matrix_count = group->node_matrix_count;
            skinning.node_matrices = (real_matrix4x3 *)group->node_matrices;
        }
        else
        {
            skinning.node_matrix_count = 1;
            skinning.node_matrices = (real_matrix4x3 *)global_identity4x3;
        }

        rasterizer_set_model_skinning(&skinning, (~(uint16_t)group->geometry_flags >> 8) & 1);

        if ( group->geometry_flags & (1u << _rasterizer_geometry_parts_define_local_nodes_bit) )
            _rasterizer_model_setupnodeparts(group->local_node_remap_table_size, group->local_node_remap_table,
                skinning.node_matrices);

        if ( group->lighting )
            rasterizer_set_model_lighting(group->lighting);
    }

    uint8_t is_first_person_effect = (group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 && group->effect.type == _render_model_effect_type_active_camouflage;
    if ( is_first_person_effect )
        rasterizer_set_frustum_z(rasterizer_globals.z_near_first_person, rasterizer_globals.z_far_first_person);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_model));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_model_zbuffer));

    rasterizer_transparent_geometry_group_draw_internal(group, 0);

    if ( is_first_person_effect )
        rasterizer_set_frustum_z(0.0f, 0.0f);
}
