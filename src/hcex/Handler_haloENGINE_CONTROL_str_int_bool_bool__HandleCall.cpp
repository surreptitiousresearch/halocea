#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, void, dsTSTRING<char>, int, bool, bool, void...>
// ::HandleCall @ 0x823C8F80
// Four-arg (string by value, int, bool, bool), void-return dispatch leaf — the LoadLevelCoop shape.
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, void,
        dsTSTRING<char>, int, bool, bool, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)retVal;
    if (argc >= 4)
    {
        bool arg3 = ds_data_call::ArgConverter<bool>::Convert(&argv[3]);
        bool arg2 = ds_data_call::ArgConverter<bool>::Convert(&argv[2]);
        int  arg1 = ds_data_call::ArgConverter<int>::Convert(&argv[1]);
        dsTSTRING<char> arg0Storage;
        dsTSTRING<char> *arg0 =
            ds_data_call::ArgConverter<dsTSTRING<char> >::Convert(&arg0Storage, &argv[0]);
        typedef void (haloENGINE_CONTROL::*BoundFn)(dsTSTRING<char>, int, bool, bool);
        BoundFn fn = reinterpret_cast<BoundFn>(this->pf);
        // DEVIATION: string param passed by value; decompiler shows the raw arg-slot pointer.
        (host->*fn)(*arg0, arg1, arg2, arg3);
    }
    else
    {
        dsTSTRING<char> msg("Not enough arguments, 4 expected");
        dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(host, msg);
    }
}
