#include <stdint.h>

#pragma once

/* Forward declarations — these Blam render types only appear behind pointers
   in the draw function-pointer signatures, so their layout is not needed here. */
typedef struct transparent_geometry_group transparent_geometry_group;
typedef struct shader shader;
typedef struct shader_model shader_model;
typedef struct vertex_buffer vertex_buffer;
typedef struct triangle_buffer triangle_buffer;

/* Per-pixel ("_pp") draw implementations. Their addresses are stored into the
   dispatch globals below; they are not called from the init path. */
extern void rasterizer_water_draw_pp(const transparent_geometry_group *group);
extern void rasterizer_environment_diffuse_light_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_environment_diffuse_texture_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void rasterizer_environment_lightmap_draw_pp(const shader *shader, short shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
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
