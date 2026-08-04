#ifndef RASTERIZER_VERTEX_SHADER_DECLARATION_INDEX_H
#define RASTERIZER_VERTEX_SHADER_DECLARATION_INDEX_H

/* Vertex-declaration slot indices passed to rasterizer_dx9_shaders_vdecl9_get().
 * DB anonymous enum _18688B2E899A91D16CBB911F4FB975D8 (verbatim). */
enum rasterizer_vertex_shader_declaration_index
{
    _vsdecl_environment = 0,
    _vsdecl_environment_c = 1,
    _vsdecl_environment_lightmap = 2,
    _vsdecl_environment_lightmap_c = 3,
    _vsdecl_model = 4,
    _vsdecl_model_c = 5,
    _vsdecl_unlit = 6,
    _vsdecl_dynamic_unlit = 7,
    _vsdecl_screen = 8,
    _vsdecl_debug = 9,
    _vsdecl_decal = 10,
    _vsdecl_detail_object = 11,
    _vsdecl_environment_ff = 12,
    _vsdecl_environment_lightmap_ff = 13,
    _vsdecl_model_uncompressed_ff = 14,
    _vsdecl_model_processed = 15,
    _vsdecl_unlit_zsprite = 16,
    _vsdecl_widget = 17,
    NUM_VERTEX_SHADER_DECLARATIONS = 18
};

#endif /* RASTERIZER_VERTEX_SHADER_DECLARATION_INDEX_H */
