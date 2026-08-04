#pragma once
/* FCOLOR — floating-point RGBA color used by the S3TC/DXT block-compression helpers
 * (Square3x3, Quantize, ClipExtrema, AllSame, EncodeBlockRGBColorKey). DB types_members-confirmed: a
 * single float[4] member, 16 bytes total. */

typedef struct FCOLOR
{
    float rgba[4];
} FCOLOR;
