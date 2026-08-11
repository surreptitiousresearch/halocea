#include <stdint.h>

#pragma once

/* Forward declarations — these Blam render types only appear behind pointers
   in the draw function-pointer signatures, so their layout is not needed here. */
typedef struct transparent_geometry_group transparent_geometry_group;
typedef struct shader shader;
typedef struct shader_model shader_model;
typedef struct vertex_buffer vertex_buffer;
typedef struct triangle_buffer triangle_buffer;

/* This header is reached by 17 pure-C TUs and, through blam_data_globals.h, by C++ TUs. Every
   name below is a flat (unmangled) symbol — the `_pp` functions in the binary's symbol table, the
   dispatch globals from their pure-C src/data storage TUs — so the C++ view must not mangle them.
   The globals need this as much as the functions do: a C++ file-scope variable is mangled too. */
#ifdef __cplusplus
extern "C" {
#endif

/* Per-pixel ("_pp") draw implementations. Their addresses are stored into the
   dispatch globals below; they are not called from the init path. */
extern void rasterizer_water_draw_pp(const transparent_geometry_group *group);
extern void rasterizer_environment_diffuse_light_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_environment_diffuse_texture_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void _rasterizer_environment_lightmap_draw_pp(const shader *shader, short shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_model_draw_environment_shader_pp(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_model_draw_model_shader_pp(const shader *shader_base, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_glass_draw_tint_pp(const transparent_geometry_group *group);
extern void rasterizer_glass_draw_reflection_pp(const transparent_geometry_group *group, int16_t reflection_mode);
extern void rasterizer_glass_draw_diffuse_pp(const transparent_geometry_group *group);

/* Dispatch globals (selected drawing implementation per subsystem). */
extern void (*rasterizer_water_draw)(const transparent_geometry_group *);
extern void (*rasterizer_environment_diffuse_light_draw_function)(const shader *, short, int, int, int, const vertex_buffer *);
extern void (*rasterizer_environment_diffuse_texture_draw_function)(const shader *, short, int, int, int, const vertex_buffer *);
extern void (*rasterizer_environment_lightmaps_draw_function)(const shader *, short, int, int, int, const vertex_buffer *);
extern void (*rasterizer_model_draw_environment_shader)(const shader *, short, const triangle_buffer *, int, int, const vertex_buffer *, int);
extern void (*rasterizer_model_draw_model_shader)(const shader *, short, const triangle_buffer *, int, int, const vertex_buffer *, int);
extern void (*rasterizer_glass_draw_tint)(const transparent_geometry_group *);
extern void (*rasterizer_glass_draw_reflection)(const transparent_geometry_group *, short);
extern void (*rasterizer_glass_draw_diffuse)(const transparent_geometry_group *);

#ifdef __cplusplus
}
#endif
