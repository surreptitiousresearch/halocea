#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::OnApproach(dsTSTRING<char> &, bool) @ 0x8329AA28
// ?OnApproach@aiBRAIN@@QAAXAAV?$dsTSTRING@D@@_N@Z
//
// Fire this brain's descriptor OnApproach SSL event, choosing the enter- or leave-event id and
// passing the approach `name` string as the boxed event payload.
void aiBRAIN::OnApproach(dsTSTRING<char> &name, bool isEnter)
{
    // Box the name into a dsDATA event value (StoreValue makes its own ref-counted copy).
    dsDATA event;
    event.type = nullptr;
    event.StoreValue<dsTSTRING<char> >(name);

    int eventId = isEnter ? aiBRAIN_DESC_SslEvOnApproachEnter(spDesc.pointee)
                          : aiBRAIN_DESC_SslEvOnApproachLeave(spDesc.pointee);

    // RAII: the returned sslERROR temporary and `event` release themselves at scope end.
    sslObject.HandleEvent(eventId, event);
}
