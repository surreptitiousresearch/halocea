#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, dsTSTRING<char>, int, bool, void...>::HandleCall
// @ 0x823C8E48
// Three-arg (string by value, int, bool), void-return dispatch leaf — the LoadLevelSolo shape.
// Args are unmarshalled in reverse (matching the PPC codegen); on too-few args, raise the standard
// error.
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, void,
        dsTSTRING<char>, int, bool, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)retVal;
    if (argc >= 3)
    {
        bool arg2 = ds_data_call::ArgConverter<bool>::Convert(&argv[2]);
        int  arg1 = ds_data_call::ArgConverter<int>::Convert(&argv[1]);
        dsTSTRING<char> arg0Storage;
        dsTSTRING<char> *arg0 =
            ds_data_call::ArgConverter<dsTSTRING<char> >::Convert(&arg0Storage, &argv[0]);
        typedef void (haloENGINE_CONTROL::*BoundFn)(dsTSTRING<char>, int, bool);
        BoundFn fn = reinterpret_cast<BoundFn>(this->pf);
        // DEVIATION: the string param is passed by value; the decompiler shows the raw arg-slot
        // pointer (PPC by-value ABI). Passing *arg0 is the source-level equivalent.
        (host->*fn)(*arg0, arg1, arg2);
    }
    else
    {
        dsTSTRING<char> msg("Not enough arguments, 3 expected");
        dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(host, msg);
    }
}
