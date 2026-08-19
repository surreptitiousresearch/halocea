/* rasterizer_screen_effect_set_texture_transforms @0x83685A78 — build the four texture-coordinate
 * transforms (2 rows of 4 floats each, vertex-shader constants c13..c20) for one pass of the
 * full-screen "filth" post-process quad: stage 0 = convolution mask, stage 1 = video scanline map,
 * stage 2 = video noise map, stage 3 = screen/zoom. Each stage maps the quad onto its bitmap with a
 * ((map_dim + 1) - target_dim) * scale * 0.5 centring offset in a 640x480 authoring space (or the
 * live viewport when that stage's map is absent). convolution_type 1 shifts every stage by
 * convolution_radius texels (radial-blur pass offsets); convolution_type 2 zooms the stages instead;
 * otherwise video passes >= 1 jitter the noise map by a random whole-texture offset. ignore_stage_0
 * shifts the scanline/noise transforms down one stage.
 *
 * Reconstructed by per-instruction disasm decode (Hex-Rays "local variable allocation has failed";
 * the OVERLAPPED int64 register-pair puns hid every width/height pairing). DEVIATIONS, all
 * disasm-verified:
 * - absent maps are substituted by a fake 48-byte bitmap_data built on the stack: 'bitm' signature,
 *   width/height = viewport dims, depth 1, format -1, flags = cf_LinearTextureAddressingZoom ? 0x10
 *   : 0, remainder zeroed (4 x std zero over +0x10..+0x2F); the stage-3 zoom transform always uses
 *   this stub.
 * - per-stage scale pair = that bitmap's flags & 0x10 (linear texture addressing) ? {1, 1}
 *   : {1/width, 1/height} (fdivs pairs at 0x83685C98/CC8/CF8/D28).
 * - the common divisors and the stage-3 dims re-read the stub's int16 fields (lhz+extsh at
 *   0x83685BB0/BEC), i.e. the viewport dims truncated to short; the target-dims pair keeps the full
 *   unsigned viewport values (clrldi+fcfid in the prologue).
 * - the convolution_type 1/2 gates test only convolution_mask != NULL (cmplwi r8 at
 *   0x83685F34/F50/FA0/FB4; the decompiler's HIDWORD(v21)), unlike the stage-0 map/target selection
 *   which also needs pass > 0 || passes == 1 || convolution_type != 0.
 * - shipped quirks kept literally: convolution_type 2 scales the scanline stage by
 *   (1 - 0.0f / scanline_dim) (a real fdivs of literal zero, 0x83685FEC/FF0) and offsets it by
 *   scale * 0.0f; the stage-3 base offsets are ((dim + 1) - dim) * scale * 0.5 with both dims the
 *   same value (not folded).
 * - the upload count/mask (decompiler rendered 0x1C000000 / a HIDWORD pun) are li r6,8 / sldi 7<<58:
 *   D3DDevice_SetVertexShaderConstantFN(device, 0xD, constants, 8, 7 << 58). */

#include <stdint.h>
#include "headers/rasterizer_screen_effect_parameters.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/d3d_boundary.h"
#include "headers/bitmap_type.h"
#include "headers/rasterizer_screen_effect_convolution_type.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

void rasterizer_screen_effect_set_texture_transforms(const rasterizer_screen_effect_parameters *parameters, int16_t pass, int16_t passes, unsigned int viewport_width, unsigned int viewport_height, uint8_t ignore_stage_0)
{
    bitmap_data screen_bitmap;
    float constants[32]; /* 4 stages x {sx,0,0,tx, 0,sy,0,ty} -> vertex-shader constants c13..c20 */

    screen_bitmap.signature = 0x6269746D; /* 'bitm' */
    screen_bitmap.width = (short)viewport_width;
    screen_bitmap.height = (short)viewport_height;
    screen_bitmap.depth = 1;
    screen_bitmap.type = bitmap_type_2d;
    screen_bitmap.format = -1;
    screen_bitmap.flags = cf_LinearTextureAddressingZoom ? (1u << _bitmap_linear_bit) : 0;
    screen_bitmap.registration_point.__s1.x = 0;
    screen_bitmap.registration_point.__s1.y = 0;
    screen_bitmap.mipmap_count = 0;
    screen_bitmap.mipmap_pad = 0;
    screen_bitmap.pixels_offset = 0;
    screen_bitmap.pixels_size = 0;
    screen_bitmap.tag_index = 0;
    screen_bitmap.cache_block_index = 0;
    screen_bitmap.hardware_format = 0;
    screen_bitmap.base_address = 0;

    int convolution_active = parameters->convolution_mask != 0
        && (pass > 0 || passes == 1 || parameters->convolution_type != _rasterizer_screen_effect_convolution_type_none);

    const bitmap_data *convolution_map = convolution_active ? parameters->convolution_mask : &screen_bitmap;
    const bitmap_data *scanline_map = parameters->video_on ? parameters->video_scanline_map : &screen_bitmap;
    const bitmap_data *noise_map = parameters->video_on ? parameters->video_noise_map : &screen_bitmap;

    float convolution_width = (float)convolution_map->width;
    float convolution_height = (float)convolution_map->height;
    float scanline_width = (float)scanline_map->width;
    float scanline_height = (float)scanline_map->height;
    float noise_width = (float)noise_map->width;
    float noise_height = (float)noise_map->height;
    /* the divisor/stage-3 dims come from the stub's int16 fields (viewport truncated to short) */
    float screen_width = (float)screen_bitmap.width;
    float screen_height = (float)screen_bitmap.height;

    float viewport_size[2];
    viewport_size[0] = (float)viewport_width;
    viewport_size[1] = (float)viewport_height;

    float authored_size[2]; /* the 640x480 render size the screen-effect bitmaps were authored for */
    authored_size[0] = 640.0f;
    authored_size[1] = 480.0f;

    float convolution_scale[2], scanline_scale[2], noise_scale[2], zoom_scale[2];

    if ( convolution_map->flags & (1u << _bitmap_linear_bit) )
    {
        convolution_scale[0] = 1.0f;
        convolution_scale[1] = 1.0f;
    }
    else
    {
        convolution_scale[0] = 1.0f / convolution_width;
        convolution_scale[1] = 1.0f / convolution_height;
    }
    if ( scanline_map->flags & (1u << _bitmap_linear_bit) )
    {
        scanline_scale[0] = 1.0f;
        scanline_scale[1] = 1.0f;
    }
    else
    {
        scanline_scale[0] = 1.0f / scanline_width;
        scanline_scale[1] = 1.0f / scanline_height;
    }
    if ( noise_map->flags & (1u << _bitmap_linear_bit) )
    {
        noise_scale[0] = 1.0f;
        noise_scale[1] = 1.0f;
    }
    else
    {
        noise_scale[0] = 1.0f / noise_width;
        noise_scale[1] = 1.0f / noise_height;
    }
    if ( screen_bitmap.flags & (1u << _bitmap_linear_bit) )
    {
        zoom_scale[0] = 1.0f;
        zoom_scale[1] = 1.0f;
    }
    else
    {
        zoom_scale[0] = 1.0f / screen_width;
        zoom_scale[1] = 1.0f / screen_height;
    }

    const float *convolution_target = convolution_active ? authored_size : viewport_size;
    const float *scanline_target = parameters->video_on ? authored_size : viewport_size;
    const float *noise_target = parameters->video_on ? authored_size : viewport_size;

    float inv_screen_width = 1.0f / screen_width;
    float inv_screen_height = 1.0f / screen_height;

    /* stage 0: convolution mask */
    constants[0] = (convolution_scale[0] * convolution_target[0]) * inv_screen_width;
    constants[1] = 0.0f;
    constants[2] = 0.0f;
    constants[3] = (((convolution_width + 1.0f) - convolution_target[0]) * convolution_scale[0]) * 0.5f;
    constants[4] = 0.0f;
    constants[5] = (convolution_scale[1] * convolution_target[1]) * inv_screen_height;
    constants[6] = 0.0f;
    constants[7] = (((convolution_height + 1.0f) - convolution_target[1]) * convolution_scale[1]) * 0.5f;

    /* stage 1: video scanline map */
    constants[8] = (scanline_scale[0] * scanline_target[0]) * inv_screen_width;
    constants[9] = 0.0f;
    constants[10] = 0.0f;
    constants[11] = (((scanline_width + 1.0f) - scanline_target[0]) * scanline_scale[0]) * 0.5f;
    constants[12] = 0.0f;
    constants[13] = (scanline_scale[1] * scanline_target[1]) * inv_screen_height;
    constants[14] = 0.0f;
    constants[15] = (((scanline_height + 1.0f) - scanline_target[1]) * scanline_scale[1]) * 0.5f;

    /* stage 2: video noise map */
    constants[16] = (noise_scale[0] * noise_target[0]) * inv_screen_width;
    constants[17] = 0.0f;
    constants[18] = 0.0f;
    constants[19] = (((noise_width + 1.0f) - noise_target[0]) * noise_scale[0]) * 0.5f;
    constants[20] = 0.0f;
    constants[21] = (noise_scale[1] * noise_target[1]) * inv_screen_height;
    constants[22] = 0.0f;
    constants[23] = (((noise_height + 1.0f) - noise_target[1]) * noise_scale[1]) * 0.5f;

    /* stage 3: screen zoom (always the viewport stub) */
    constants[24] = zoom_scale[0];
    constants[25] = 0.0f;
    constants[26] = 0.0f;
    constants[27] = (((screen_width + 1.0f) - screen_width) * zoom_scale[0]) * 0.5f;
    constants[28] = 0.0f;
    constants[29] = zoom_scale[1];
    constants[30] = 0.0f;
    constants[31] = (((screen_height + 1.0f) - screen_height) * zoom_scale[1]) * 0.5f;

    if ( parameters->convolution_type == _rasterizer_screen_effect_convolution_type_blur )
    {
        /* radial-blur pass: push each stage's offsets by convolution_radius texels */
        float radius = parameters->convolution_radius;
        constants[3] = (parameters->convolution_mask ? 0.0f : convolution_scale[0] * radius) + constants[3];
        constants[7] = (parameters->convolution_mask ? 0.0f : convolution_scale[1] * radius) + constants[7];
        constants[11] = constants[11] - scanline_scale[0] * radius;
        constants[15] = constants[15] - scanline_scale[1] * radius;
        constants[19] = constants[19] + noise_scale[0] * radius;
        constants[23] = constants[23] - noise_scale[1] * radius;
        constants[27] = constants[27] - zoom_scale[0] * radius;
        constants[31] = constants[31] + zoom_scale[1] * radius;
    }
    else if ( parameters->convolution_type == _rasterizer_screen_effect_convolution_type_warp )
    {
        /* zoom pass ("warp"): scale each stage around its centre by convolution_radius */
        float radius = parameters->convolution_radius;
        float convolution_zoom = parameters->convolution_mask ? 0.0f : -radius;
        float screen_zoom = parameters->convolution_mask ? -radius : radius * 2.0f;

        constants[0] = (1.0f - convolution_zoom / convolution_width) * constants[0];
        constants[5] = (1.0f - convolution_zoom / convolution_height) * constants[5];
        constants[3] = (convolution_scale[0] * convolution_zoom) * 0.5f + constants[3];
        constants[7] = (convolution_scale[1] * convolution_zoom) * 0.5f + constants[7];

        /* faithful: the scanline stage's zoom amount is a literal 0.0, still divided/multiplied */
        constants[8] = (1.0f - 0.0f / scanline_width) * constants[8];
        constants[13] = (1.0f - 0.0f / scanline_height) * constants[13];
        constants[11] = scanline_scale[0] * 0.0f + constants[11];
        constants[15] = scanline_scale[1] * 0.0f + constants[15];

        constants[16] = (1.0f - radius / noise_width) * constants[16];
        constants[21] = (1.0f - radius / noise_height) * constants[21];
        constants[19] = (noise_scale[0] * radius) * 0.5f + constants[19];
        constants[23] = (noise_scale[1] * radius) * 0.5f + constants[23];

        constants[24] = (1.0f - screen_zoom * inv_screen_width) * zoom_scale[0];
        constants[29] = (1.0f - screen_zoom * inv_screen_height) * zoom_scale[1];
        constants[27] = (zoom_scale[0] * screen_zoom) * 0.5f + constants[27];
        constants[31] = (zoom_scale[1] * screen_zoom) * 0.5f + constants[31];
    }
    else if ( pass == 1 && parameters->video_on )
    {
        /* plain video pass: random whole-texture jitter of the noise map */
        constants[19] = (noise_scale[0] * real_seed_random(get_global_local_random_seed_address()))
                      * noise_width + constants[19];
        constants[23] = (noise_scale[1] * real_seed_random(get_global_local_random_seed_address()))
                      * noise_height + constants[23];
    }

    if ( ignore_stage_0 )
    {
        /* no convolution stage this pass: shift scanline -> stage 0, noise -> stage 1 */
        int slot;
        for ( slot = 0; slot < 16; ++slot )
            constants[slot] = constants[slot + 8];
    }

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, constants, 8, (uint64_t)7 << 58);
}
