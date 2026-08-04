#pragma once
/* targa_header — 18-byte TGA file header (uncompressed true-color, top-down). Packed so the on-disk size
 * is exactly 18 bytes. */

#pragma pack(push, 1)
typedef struct targa_header
{
    unsigned __int8  id_length;                    /* 0x00 */
    unsigned __int8  colormap_type;                /* 0x01 */
    unsigned __int8  image_type;                   /* 0x02 = uncompressed true-color */
    unsigned __int8  colormap_specification[5];    /* 0x03 */
    unsigned __int16 x_origin;                      /* 0x08 */
    unsigned __int16 y_origin;                      /* 0x0A */
    unsigned __int16 image_width;                   /* 0x0C */
    unsigned __int16 image_height;                  /* 0x0E */
    unsigned __int8  pixel_depth;                   /* 0x10 */
    unsigned __int8  image_descriptor;              /* 0x11 = top-down, 8 alpha bits */
} targa_header;                                     /* 18 bytes */
#pragma pack(pop)
