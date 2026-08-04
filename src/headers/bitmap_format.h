#pragma once

/* bitmap_data.format enumeration.
 * DB anonymous enum _8A9F74B45C0A0C99644BF4ABDB458B72 (types_enum_values). */
enum bitmap_format
{
    _bitmap_format_a8       = 0,
    _bitmap_format_y8       = 1,
    _bitmap_format_ay8      = 2,
    _bitmap_format_a8y8     = 3,
    _bitmap_format_unused1  = 4,
    _bitmap_format_unused2  = 5,
    _bitmap_format_r5g6b5   = 6,
    _bitmap_format_unused3  = 7,
    _bitmap_format_a1r5g5b5 = 8,
    _bitmap_format_a4r4g4b4 = 9,
    _bitmap_format_x8r8g8b8 = 10,
    _bitmap_format_a8r8g8b8 = 11,
    _bitmap_format_unused4  = 12,
    _bitmap_format_unused5  = 13,
    _bitmap_format_dxt1     = 14,
    _bitmap_format_dxt3     = 15,
    _bitmap_format_dxt5     = 16,
    _bitmap_format_p8_bump  = 17,
    NUMBER_OF_BITMAP_FORMATS = 18,
    /* DB aliases in the same enum (span of DXT-compressed formats). */
    BITMAP_FIRST_COMPRESSED_FORMAT = 14,
    BITMAP_LAST_COMPRESSED_FORMAT  = 16
};
