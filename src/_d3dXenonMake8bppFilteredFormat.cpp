// _d3dXenonMake8bppFilteredFormat @0x827B9F38 — inverse of _d3dXenonMake16bppFilteredFormat:
// map a "..._AS_16_16_16_16" filtered Xenos format back to its base 8bpp sibling. Formats outside
// that filtered range (fmt - 50 > 6) are returned unchanged.
#include "headers/GPUTEXTUREFORMAT.h"

GPUTEXTUREFORMAT _d3dXenonMake8bppFilteredFormat(GPUTEXTUREFORMAT fmt)
{
    if ((unsigned int)(fmt - GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16) > 6)
        return fmt;

    switch (fmt)
    {
    case GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16:   return GPUTEXTUREFORMAT_8_8_8_8;
    case GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16:      return GPUTEXTUREFORMAT_DXT1;
    case GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16:    return GPUTEXTUREFORMAT_DXT2_3;
    case GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16:    return GPUTEXTUREFORMAT_DXT4_5;
    case GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16: return GPUTEXTUREFORMAT_2_10_10_10;
    case GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16:  return GPUTEXTUREFORMAT_10_11_11;
    default:                                        return GPUTEXTUREFORMAT_11_11_10; // 11_11_10_AS_16_16_16_16
    }
}
