#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/ds_boundary.h"     // dsDATA_TYPE_STORAGE<T>::dataType
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, const dsTSTRING<char>&, void...>::HandleCall @ 0x823C3048
// The return-value dispatch leaf: invoke the bound haloENGINE_CONTROL member (a string getter),
// box its result as a dsTSTRING<char>-typed dsDATA, and copy it into retVal.
// This instantiation carries a non-void Ret, so it is an explicit specialization of the primary
// Handler template's inline HandleCall (which models only the no-arg/void-return shape).
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, const dsTSTRING<char> &,
        void, void, void, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)argv; (void)argc;
    // The layout-identical member-fn-ptr slot is cast back to the bound getter's true signature.
    typedef const dsTSTRING<char> &(haloENGINE_CONTROL::*BoundGetter)();
    BoundGetter getter = reinterpret_cast<BoundGetter>(this->pf);
    const dsTSTRING<char> *result = &(host->*getter)();

    dsDATA boxed;
    boxed.type = dsDATA_TYPE_STORAGE<dsTSTRING<char> >::dataType;
    boxed.type->ConstructFromObj(&boxed.storage, result);
    // DEVIATION: decompiler renders this as dsDATA::operator=(retVal, &boxed); that assignment
    // operator is the StoreValue(const dsDATA&) copy-assign (same target 0x823B3FA0).
    retVal->StoreValue(boxed);
    boxed.~dsDATA();
}
