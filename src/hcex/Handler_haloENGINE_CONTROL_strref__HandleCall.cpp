#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, const dsTSTRING<char>&, void...>::HandleCall
// @ 0x823C90B0
// One-string-arg (by const-ref), void-return dispatch leaf: unmarshal argv[0] into a temp string,
// forward its reference to the bound member, then release the temp (decompiler inlines the buffer
// refcount teardown — modeled here by the temp's scope-end dtor).
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, void,
        const dsTSTRING<char> &, void, void, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)retVal;
    if (argc >= 1)
    {
        dsTSTRING<char> arg0Storage;
        const dsTSTRING<char> *arg0 =
            ds_data_call::ArgConverter<const dsTSTRING<char> &>::Convert(&arg0Storage, &argv[0]);
        typedef void (haloENGINE_CONTROL::*BoundFn)(const dsTSTRING<char> &);
        BoundFn fn = reinterpret_cast<BoundFn>(this->pf);
        (host->*fn)(*arg0);
    }
    else
    {
        dsTSTRING<char> msg("Not enough arguments, 1 expected");
        dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(host, msg);
    }
}
