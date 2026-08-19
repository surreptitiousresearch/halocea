/* ?Default@ENGINE_TEX_DESC@@SAABU1@XZ @0x82C66A90 */
#include "headers/ws/txm/ENGINE_TEX_DESC.h"
#include <string.h>

// 0x82C66AB4 -- function-local-static singleton: zero-initialised, then usage/strmPriority
// patched to their "no usage bits, normal streaming priority" defaults on first call.
const ENGINE_TEX_DESC &ENGINE_TEX_DESC::Default()
{
    static ENGINE_TEX_DESC def_desc;
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
        memset(&def_desc, 0, sizeof(def_desc));
        def_desc.usage.maps = 0;
        def_desc.usage.numMaps = 0;
        def_desc.strmPriority = ETSP_NORMAL;
    }

    return def_desc;
}
