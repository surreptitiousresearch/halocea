#pragma once
/* rasterizer_lens_flare_submit_parameters — one lens flare submitted for the frame (40 bytes). Layout from
 * the database; the compressed geometry/color fields are stored as packed ints decoded at submit time. */

#include <stdint.h>
#include "real_point3d.h"

struct lens_flare_definition;

typedef struct rasterizer_lens_flare_submit_parameters
{
    struct lens_flare_definition *definition;     /* 0x00 */
    real_point3d  position;                       /* 0x04 */
    unsigned int  compressed_direction;           /* 0x10 */
    unsigned int  compressed_up;                  /* 0x14 */
    unsigned int  compressed_light_color;         /* 0x18 */
    int16_t       light_identifier;               /* 0x1C */
    int16_t       light_index;                    /* 0x1E */
    int16_t       lens_flare_index;               /* 0x20 */
    unsigned char compressed_window_index;        /* 0x22 */
    unsigned char compressed_light_scale;         /* 0x23 */
    int           internal__occlusion_pixels;     /* 0x24 */
} rasterizer_lens_flare_submit_parameters;        /* 0x28 */

extern rasterizer_lens_flare_submit_parameters local_lens_flare_parameters[1024];
extern int local_lens_flare_count;
