#pragma once
#include <stdint.h>
/* targa_header — 18-byte TGA file header (uncompressed true-color, top-down). Packed so the on-disk size
 * is exactly 18 bytes. */

#pragma pack(push, 1)
typedef struct targa_header
{
    uint8_t  id_length;                    /* 0x00 */
    uint8_t  colormap_type;                /* 0x01 */
    uint8_t  image_type;                   /* 0x02 = uncompressed true-color */
    uint8_t  colormap_specification[5];    /* 0x03 */
    uint16_t x_origin;                      /* 0x08 */
    uint16_t y_origin;                      /* 0x0A */
    uint16_t image_width;                   /* 0x0C */
    uint16_t image_height;                  /* 0x0E */
    uint8_t  pixel_depth;                   /* 0x10 */
    uint8_t  image_descriptor;              /* 0x11 = top-down, 8 alpha bits */
} targa_header;                                     /* 18 bytes */
#pragma pack(pop)
