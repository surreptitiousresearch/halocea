#include "headers/ws/txm/txm_functions.h"
#include "headers/ws/ds/ds_assert_boundary.h"
#include "headers/pct_format.h"

// 0x827BA828 -- pctFmt is a PCT_FORMAT code (DB anon enum $51480BAEEE82B19C82C3F71B9FE6487B).
// Cases 0..37 use the named PCT_FORMAT_* labels; cases 38..48 are the driver's HDR/float format
// extension past PCT_FORMAT_MAX (no DB names -- kept as raw case labels, see pct_format.h).
_D3DFORMAT txmPctFmtToD3dFmt(int pctFmt, int isSwizzledFormatReq)
{
    switch (pctFmt)
    {
        case PCT_FORMAT_ARGB8888:
            return isSwizzledFormatReq ? D3DFMT_A8R8G8B8 : D3DFMT_LIN_A8R8G8B8;
        case PCT_FORMAT_A8:
            return D3DFMT_LIN_L8;
        case PCT_FORMAT_DXT1_OPAQUE:
        case PCT_FORMAT_DXT1_TRANSPARENT:
            return D3DFMT_LIN_DXT1;
        case PCT_FORMAT_DXT2:
        case PCT_FORMAT_DXT3:
            return D3DFMT_LIN_DXT2; // DEVIATION: DB enum aliases LIN_DXT2/LIN_DXT3 to the same value (438304851)
        case PCT_FORMAT_DXT4:
        case PCT_FORMAT_DXT5:
            return D3DFMT_LIN_DXT4; // DEVIATION: DB enum aliases LIN_DXT4/LIN_DXT5 to the same value (438304852)
        case PCT_FORMAT_XRGB8888:
            return isSwizzledFormatReq ? D3DFMT_X8R8G8B8 : D3DFMT_LIN_X8R8G8B8;
        case PCT_FORMAT_DXN:
            return D3DFMT_LIN_DXN;
        case PCT_FORMAT_DXT5A:
            return D3DFMT_LIN_DXT5A;
        case 38: // past PCT_FORMAT_MAX: driver HDR/float format extension (no DB name)
            return D3DFMT_LIN_A32B32G32R32F;
        case 39:
            return D3DFMT_A16B16G16R16F;
        case 40:
            return D3DFMT_A16B16G16R16;
        case 41:
            return D3DFMT_LIN_G32R32F;
        case 42:
            return D3DFMT_LIN_G16R16F;
        case 43:
            return D3DFMT_LIN_G16R16;
        case 44:
            return D3DFMT_L16;
        case 45:
            return D3DFMT_D24FS8;
        case 47:
            // Unsupported pct format -- fires a strong-assert crash in debug builds.
            if (!IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "0",
                    "D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\d3d_txm_8.cpp",
                    438,
                    empty_string);
            return (_D3DFORMAT)-1;
        case 48:
            return D3DFMT_R32F;
        default:
            return (_D3DFORMAT)-1;
    }
}
