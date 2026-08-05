#include "../../headers/ws/strm/strmRESOURCE_PACK_S3D.h"
#include "../../headers/ws/strm/strm_boundary.h"

// strmRESOURCE_PACK_S3D<strmRESOURCE,strmRESOURCE_TEXTURE>::IsReadFromPack @ 0x8266F6E0
// Reads the "Preload.usePaks" config key once (a Meyers-singleton function-local static caches
// the resolved dsDATA), then returns whether it holds exactly the int value 1. Every branch that
// can't produce a definite answer (gCfg not yet live, key missing, or an explicit-conversion
// failure) logs a warning and returns false.
template<>
bool strmRESOURCE_PACK_S3D<strmRESOURCE, strmRESOURCE_TEXTURE>::IsReadFromPack()
{
    if (!gCfg) {
        _apLog("~Cfg,Warning~The key <Preload.usePaks> has been accessed before config init");
        return false;
    }

    static dsDATA value = gCfg->GetRawValue("Preload.usePaks");

    if (!IGNORE_STRONG_ASSERT) {
        // DEVIATION: the decompiler's asserts compare `value.type` against
        // dsDATA_TYPE_STORAGE<psSECTION>::dataType / <psARRAY>::dataType directly (the two dsDATA
        // shapes GetRawValue's psSECTION fallback can never legitimately return); reproduced
        // verbatim as the two-step check the binary performs.
        if (value.type == dsDATA_TYPE_STORAGE<psSECTION>::dataType)
            STRONG_ASSERT_DUMMY().Crash("!value.IsType<psSECTION>()",
                "d:\\projects\\code\\common\\incl.sys\\drv\\strm_resource.hpp", 17, empty_string);
        if (value.type == dsDATA_TYPE_STORAGE<psARRAY>::dataType)
            STRONG_ASSERT_DUMMY().Crash("!value.IsType<psARRAY>()",
                "d:\\projects\\code\\common\\incl.sys\\drv\\strm_resource.hpp", 17, empty_string);
    }

    if (!value.type) {
        _apLog("~Cfg,Warning~Couldnt get value <Preload.usePaks>, key doesnt exist in config");
        return false;
    }

    int intValue;
    if (value.GetValue<int>(intValue, DSD_CONV_EXPLICIT))
        return intValue == 1;

    _apLog("~Cfg,Warning~Couldnt get value <Preload.usePaks>, explicit conversion failed");
    return false;
}
