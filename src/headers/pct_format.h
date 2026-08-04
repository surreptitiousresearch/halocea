#pragma once

/* PCT (picture) pixel-format codes. Values verbatim from the DB anonymous enum
 * _51480BAEEE82B19C82C3F71B9FE6487B (the pct texture-format enumeration).
 * NOTE: the enum terminates at PCT_FORMAT_MAX (38); the shipped d3d_8 render
 * driver's format tables extend past MAX with HDR/float formats (39..48) that
 * have no DB name — those are left as raw case labels at their use sites. */

enum PCT_FORMAT
{
    PCT_FORMAT_UNKNOWN          = -1,
    PCT_FORMAT_ARGB8888         = 0,
    PCT_FORMAT_ARGB4444         = 1,
    PCT_FORMAT_RGB888           = 2,
    PCT_FORMAT_A8               = 3,
    PCT_FORMAT_RGB332           = 4,
    PCT_FORMAT_RGB565           = 5,
    PCT_FORMAT_ARGB8332         = 6,
    PCT_FORMAT_ARGB1555         = 7,
    PCT_FORMAT_I8               = 8,
    PCT_FORMAT_AI44             = 9,
    PCT_FORMAT_AI88             = 10,
    PCT_FORMAT_UV88             = 11,
    PCT_FORMAT_DXT1_OPAQUE      = 12,
    PCT_FORMAT_DXT1_TRANSPARENT = 13,
    PCT_FORMAT_DXT2             = 14,
    PCT_FORMAT_DXT3             = 15,
    PCT_FORMAT_DXT4             = 16,
    PCT_FORMAT_DXT5             = 17,
    PCT_FORMAT_P8               = 18,
    PCT_FORMAT_P4               = 19,
    PCT_FORMAT_AP8              = 20,
    PCT_FORMAT_AP4              = 21,
    PCT_FORMAT_XRGB8888         = 22,
    PCT_FORMAT_ARGB7888R        = 23,
    PCT_FORMAT_ARGB1555R        = 24,
    PCT_FORMAT_RGB555R          = 25,
    PCT_FORMAT_RGB888R          = 26,
    PCT_FORMAT_SWIZ_ARGB8888    = 27,
    PCT_FORMAT_SWIZ_ARGB4444    = 28,
    PCT_FORMAT_SWIZ_RGB888      = 29,
    PCT_FORMAT_SWIZ_RGB565      = 30,
    PCT_FORMAT_SWIZ_ARGB1555    = 31,
    PCT_FORMAT_SWIZ_UV88        = 32,
    PCT_FORMAT_SWIZ_XRGB8888    = 33,
    PCT_FORMAT_SWIZ_P8          = 34,
    PCT_FORMAT_SWIZ_AP8         = 35,
    PCT_FORMAT_DXN              = 36,
    PCT_FORMAT_DXT5A            = 37,
    PCT_FORMAT_MAX              = 38,
};
