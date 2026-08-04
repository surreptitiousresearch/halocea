#ifndef RASTERIZER_VERTEX_TYPE_H
#define RASTERIZER_VERTEX_TYPE_H

/* rasterizer vertex buffer type enum — headers_ref _CF57790460AB4D75BF9117BCF4018910 */
enum rasterizer_vertex_type
{
    _rasterizer_vertex_type_environment_uncompressed          = 0,
    _rasterizer_vertex_type_environment_compressed            = 1,
    _rasterizer_vertex_type_environment_lightmap_uncompressed = 2,
    _rasterizer_vertex_type_environment_lightmap_compressed   = 3,
    _rasterizer_vertex_type_model_uncompressed                = 4,
    _rasterizer_vertex_type_model_compressed                  = 5,
    _rasterizer_vertex_type_dynamic_unlit                     = 6,
    _rasterizer_vertex_type_dynamic_lit                       = 7,
    _rasterizer_vertex_type_dynamic_screen                    = 8,
    _rasterizer_vertex_type_debug                             = 9,
    _rasterizer_vertex_type_decal                             = 10,
    _rasterizer_vertex_type_detail_object                     = 11,
    _rasterizer_vertex_type_environment_uncompressed_ff       = 12,
    _rasterizer_vertex_type_environment_lightmap_uncompressed_ff = 13,
    _rasterizer_vertex_type_model_uncompressed_ff             = 14,
    _rasterizer_vertex_type_model_processed                   = 15,
    _rasterizer_vertex_type_unlit_zsprite                     = 16,
    _rasterizer_vertex_type_widget                            = 17,
    NUMBER_OF_RASTERIZER_VERTEX_TYPES                         = 18,
};

#endif /* RASTERIZER_VERTEX_TYPE_H */

// rasterizer_vertex_type: DB-verified via types_enum_values _CF57790460AB4D75BF9117BCF4018910 (anonymous compiled enum, names sans leading _)
