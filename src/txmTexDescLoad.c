#include "headers/ws/txm/txm_functions.h"
#include "headers/ws/txm/TEX_USAGE_PROCESSOR.h"

extern "C" int stricmp(const char *a, const char *b);

// 0x82C66648 -- reconstructed against the by-value-with-default psSECTION_INTERFACE::GetInt/
// GetBool/GetFloat overloads (psSECTION_INTERFACE.h) rather than the decompiler's literal call
// sites: those showed each accessor taking 8-16 args (v33/v37/v41/.../g_PFP[208]) which is a
// known stack-slot-reuse decompiler artifact (the same pattern already documented as a DEVIATION
// in psSYSTEM::_LoadPsFromFile) -- the real ABI per the mangled callee names
// (?GetInt@psSECTION_INTERFACE@@QBAHPBDHK@Z etc.) is (key, def, flags).
bool txmTexDescLoad(ENGINE_TEX_DESC *result, const psSECTION_INTERFACE &src)
{
    if (src.pData == nullptr)
        return false;

    float version = 0.0f;
    src.GetFloat("version", &version, 0);
    if (version != 1.0f)
        return true; // no "version 1" section -- leave *result untouched, report success

    char strBuf[512];
    if (!src.GetString("usage", strBuf, 512, 0) || !txmParseTexUsage(result->usage, strBuf))
        return false;

    result->mapping.lod_bias = 0.0f;
    result->mapping.lod_bias_PS3_cached = 0;
    result->mapping.anisotropy = 0;
    result->mapping.addressU = ETADDR_WRAP;
    result->mapping.addressV = ETADDR_WRAP;

    psSECTION_INTERFACE mapping; // psSECTION_INTERFACE has no data beyond the psSECTION base
    src.GetSection("mapping", &mapping, 0);
    if (!mapping.IsEmpty())
    {
        result->mapping.lod_bias = mapping.GetFloat("lod_bias", result->mapping.lod_bias, 0);
        result->mapping.lod_bias_PS3_cached = (int)(result->mapping.lod_bias * 256.0f) & 0x1FFF;
        result->mapping.anisotropy = mapping.GetInt("anisotropy", result->mapping.anisotropy, 0);

        if (mapping.GetString("address_u", strBuf, 512, 0))
            GetTexAddressModeFromString(&result->mapping.addressU, strBuf);
        if (mapping.GetString("address_v", strBuf, 512, 0))
            GetTexAddressModeFromString(&result->mapping.addressV, strBuf);
    }
    // mapping is destroyed automatically at scope exit (psSECTION's RAII dtor releases the
    // section reference) -- the decompiler's explicit ~psSECTION call is that same teardown.

    result->rendering.detailDensity = 1.0f;
    result->rendering.linearRGB = false;
    result->rendering.detailScale = 1.0f;
    result->rendering.akillRef = 128;
    result->rendering.hdrScale = 1.0f;
    result->rendering.smHi = false;

    psSECTION_INTERFACE rendering;
    src.GetSection("rendering", &rendering, 0);
    if (!rendering.IsEmpty())
    {
        result->rendering.akillRef = rendering.GetInt("akill_ref", result->rendering.akillRef, 0);
        result->rendering.linearRGB = rendering.GetBool("linear_rgb", 0, 0) != 0;
        result->rendering.smHi = rendering.GetBool("sm_hi", 0, 0) != 0;
        result->rendering.detailDensity = rendering.GetFloat("detail_density", result->rendering.detailDensity, 0);
        result->rendering.detailScale = rendering.GetFloat("detail_scale", result->rendering.detailScale, 0);
        result->rendering.hdrScale = rendering.GetFloat("hdr_scale", result->rendering.hdrScale, 0);
    }
    // rendering destroyed automatically at scope exit (mirrors the decompiler's explicit
    // ~psSECTION(&v65) call).

    result->isUltraHiRes = src.GetBool("isUltraHiRes", 0, 0) != 0;
    result->strmNoLowRes = src.GetBool("strm_no_lowres", 0, 0) != 0;

    result->strmPriority = ETSP_NORMAL;
    if (src.GetString("strm_priority", strBuf, 512, 0))
    {
        if (!stricmp(strBuf, "not_stream"))
            result->strmPriority = ETSP_NOT_STREAM;
        else if (!stricmp(strBuf, "high"))
            result->strmPriority = ETSP_HIGH;
        else
            result->strmPriority = (stricmp(strBuf, "low") != 0) ? ETSP_NORMAL : ETSP_LOW;
    }

    return true;
}
