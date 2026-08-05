#pragma once
#include <stdint.h>
/* rasterizer_environment_light_globals — module-scope state shared by the environment dynamic-light passes
 * (rasterizer_environment_*_light_begin/draw). vsh_constants__pointlight is the vertex-shader constant block
 * uploaded for a point/spot light (position+inverse-radius in [0..3], two identity matrices in [4..19]). The
 * two 0x844A5Bxx values keep their database (address-derived) names: environment_specular_light_active flags whether the active
 * specular light is a spot light, rasterizer_specular_distance_attenuation_index caches the distance-attenuation map tag index. */

extern float vsh_constants__pointlight[20];
extern float specular_light_brightness;
extern int16_t   specular_light_vertex_shader_permutation_index;
extern unsigned char environment_specular_light_active;
extern int   rasterizer_specular_distance_attenuation_index;
