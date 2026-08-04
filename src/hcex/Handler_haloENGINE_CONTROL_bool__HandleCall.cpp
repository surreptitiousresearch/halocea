#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, bool, void...>::HandleCall @ 0x823C9398
// One-bool-argument, void-return dispatch leaf: unmarshal argv[0] as bool and forward to the bound
// haloENGINE_CONTROL member; on too-few args, raise the standard error.
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, void,
        bool, void, void, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)retVal;
    if (argc >= 1)
    {
        bool arg0 = ds_data_call::ArgConverter<bool>::Convert(&argv[0]);
        typedef void (haloENGINE_CONTROL::*BoundFn)(bool);
        BoundFn fn = reinterpret_cast<BoundFn>(this->pf);
        (host->*fn)(arg0);
    }
    else
    {
        // DEVIATION: decompiler inlines UnsafeInit + manual buffer refcount teardown; modeled with
        // the equivalent single-arg dsTSTRING ctor (body is UnsafeInit(s,-1,0)) whose dtor frees it.
        dsTSTRING<char> msg("Not enough arguments, 1 expected");
        dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(host, msg);
    }
}
