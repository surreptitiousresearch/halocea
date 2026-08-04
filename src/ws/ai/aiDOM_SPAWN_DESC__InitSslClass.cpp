#include "../../headers/ws/ai/aiDOM_SPAWN_DESC.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiDOM_SPAWN_DESC::InitSslClass(const dsTSTRING<char>&) @ 0x832A1EB8
// ?InitSslClass@aiDOM_SPAWN_DESC@@UAAHABV?$dsTSTRING@D@@@Z (virtual)
// Run the base descriptor's script-class init; on success, unshare the script class exactly once.
int aiDOM_SPAWN_DESC::InitSslClass(const dsTSTRING<char> &hintErr)
{
    int result = this->gsDOM_SPAWN_DESC::InitSslClass(hintErr);
    if (!result)
        return result;

    if (!this->isSslClassInited.val)
    {
        sslERROR err; // scoped error accumulator (constructed/destroyed as in the original)
        this->isSslClassInited.val = true;
        this->UnshareSslClass(false, false);
    }
    return 1;
}
