#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(instance, error_msg) @ 0x823C6B40
// Mark `instance` as the current error owner and overwrite the process-wide error-string slot.
// DEVIATION: the decompiler inlines GetCurrentInst()'s static write (`...::inst = instance`);
// modeled here through the SetCurrentInst boundary accessor that owns that slot.
template<>
void dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(
    const haloENGINE_CONTROL *instance, const dsTSTRING<char> &error_msg)
{
    dsERROR_HANDLER<haloENGINE_CONTROL>::SetCurrentInst(instance);
    dsERROR_HANDLER<haloENGINE_CONTROL>::GetErrorStr() = error_msg;
}
