#pragma once

/* 120-byte cinematic screen-effect ("filth") state block: the live parameter set the rasterizer reads,
   followed by the script-controlled interpolation endpoints (paired [from,to] floats) and the near-clip
   distance. Uses a struct tag (not a typedef) so the global pointer may legally share the name
   `cinematic_screen_effect_globals`, matching the database. */

#include "rasterizer_screen_effect_parameters.h"

struct cinematic_screen_effect_globals
{
    rasterizer_screen_effect_parameters parameters;                  /* 0x00 */
    unsigned char  has_control;                                      /* 0x38 */
    unsigned char  initialized;                                     /* 0x39 */
    unsigned char  _pad3A[2];                                       /* 0x3A */
    float          convolution_radius[2];                           /* 0x3C */
    float          convolution_time[2];                             /* 0x44 */
    float          filter_light_enhancement_intensity[2];           /* 0x4C */
    float          filter_desaturation_intensity[2];                /* 0x54 */
    float          filter_time[2];                                  /* 0x5C */
    float          script_values[4];                                /* 0x64 */
    float          near_clip_distance;                              /* 0x74 */
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct cinematic_screen_effect_globals *cinematic_screen_effect_globals;

#ifdef __cplusplus
}
#endif
