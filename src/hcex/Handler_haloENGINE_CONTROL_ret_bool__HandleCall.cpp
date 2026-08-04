#include "../headers/hcex/ssl_exporter/Handler.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/ds_boundary.h"     // dsDATA_TYPE_STORAGE<T>::dataType
#include "../headers/hcex/haloENGINE_CONTROL.h"

// ds_data_call::Handler<0, haloENGINE_CONTROL, bool, void...>::HandleCall @ 0x823C4660
// No-arg / bool-return dispatch leaf: invoke the bound haloENGINE_CONTROL predicate member, box its
// bool result as a bool-typed dsDATA, and copy it into retVal. Non-void Ret, so this is an explicit
// specialization of the primary Handler template's inline (void-return) HandleCall.
template<>
void ds_data_call::Handler<0, haloENGINE_CONTROL, bool,
        void, void, void, void, void, void>::HandleCall(
            haloENGINE_CONTROL *host, dsDATA *argv, int argc, dsDATA *retVal)
{
    (void)argv; (void)argc;
    typedef bool (haloENGINE_CONTROL::*BoundGetter)();
    BoundGetter getter = reinterpret_cast<BoundGetter>(this->pf);
    bool result = (host->*getter)();

    dsDATA boxed;
    boxed.type = dsDATA_TYPE_STORAGE<bool>::dataType;
    boxed.type->ConstructFromObj(&boxed.storage, &result);
    // DEVIATION: decompiler renders the store as dsDATA::operator=(retVal, &boxed); that operator is
    // the StoreValue(const dsDATA&) copy-assign (same target 0x823B3FA0).
    retVal->StoreValue(boxed);
    boxed.~dsDATA();
}
