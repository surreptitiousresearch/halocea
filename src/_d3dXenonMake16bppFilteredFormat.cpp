// _d3dXenonMake16bppFilteredFormat @0x827B9EA0 — map a base Xenos texture format to the
// hardware "..._AS_16_16_16_16" sibling used for gamma-correct 16bpp filtering. Formats without a
// filtered sibling are returned unchanged.
#include "headers/GPUTEXTUREFORMAT.h"

GPUTEXTUREFORMAT _d3dXenonMake16bppFilteredFormat(GPUTEXTUREFORMAT fmt)
{
    switch (fmt)
    {
    case GPUTEXTUREFORMAT_8_8_8_8:    return GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_2_10_10_10: return GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_10_11_11:   return GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_11_11_10:   return GPUTEXTUREFORMAT_11_11_10_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_DXT1:       return GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_DXT2_3:     return GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16;
    case GPUTEXTUREFORMAT_DXT4_5:     return GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16;
    default:                          return fmt;
    }
}
