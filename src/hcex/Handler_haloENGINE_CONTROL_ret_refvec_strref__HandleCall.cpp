#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/ds_boundary.h"     // dsDATA_TYPE_STORAGE<T>::dataType
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsERROR_HANDLER.h"
#include "../headers/ws/ds/REF_TYPE.h"
#include "../headers/ws/ds/dsREF_VECTOR.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, ds_data::REF_TYPE<dsREF_VECTOR>,
//                       const dsTSTRING<char>&, void...>::HandleCall @ 0x823C91F0
// One-string-arg, REF_TYPE<dsREF_VECTOR>-return dispatch leaf (the CreateCheckpointList shape):
// unmarshal argv[0], invoke the bound getter (returns a ref-vector handle by value / sret), box the
// handle into retVal, then release the returned handle and the arg temp.
// DEVIATION: the DB prototype types the host as ds_data::REF_TYPE<dsREF_VECTOR>* (a PPC sret ABI
// artifact — the sret slot and `this` share a register); the real host is haloENGINE_CONTROL*, as
// the inner SetError(pObj) call confirms.
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, ds_data::REF_TYPE<dsREF_VECTOR>,
        const dsTSTRING<char> &, void, void, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    if (argc >= 1)
    {
        dsTSTRING<char> arg0Storage;
        const dsTSTRING<char> *arg0 =
            ds_data_call::ArgConverter<const dsTSTRING<char> &>::Convert(&arg0Storage, &argv[0]);
        typedef ds_data::REF_TYPE<dsREF_VECTOR> (haloENGINE_CONTROL::*BoundFn)(const dsTSTRING<char> &);
        BoundFn fn = reinterpret_cast<BoundFn>(this->pf);
        ds_data::REF_TYPE<dsREF_VECTOR> result = (host->*fn)(*arg0);

        dsDATA boxed;
        boxed.type = dsDATA_TYPE_STORAGE<ds_data::REF_TYPE<dsREF_VECTOR> >::dataType;
        boxed.type->ConstructFromObj(&boxed.storage, &result);
        retVal->StoreValue(boxed);
        boxed.~dsDATA();
        result.Release(result.m_pData);
    }
    else
    {
        dsTSTRING<char> msg("Not enough arguments, 1 expected");
        dsERROR_HANDLER<haloENGINE_CONTROL>::SetError(host, msg);
    }
}
