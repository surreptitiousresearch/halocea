#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"

// 0x8323DD60 — aiDEF_CDT_SYS::AnalyzeResults. Walk the in-flight requests: while a request still
// has time to live, analyse it (virtual); once its destroy timer has decayed, erase it from the
// map and destroy the request object (its scalar-deleting dtor, vtable slot 0).
void aiDEF_CDT_SYS::AnalyzeResults()
{
    ReqIter it(dstReqs.Begin());
    while ( !it.IsDone() )
    {
        aidr::DR *dr = *it.value();
        if ( dr->destroyTimer >= 0.000001f )
        {
            dr->__vftable->AnalyzeResults(dr);
            ++it;
        }
        else
        {
            it = dstReqs.Erase(it);
            // scalar-deleting dtor (vtable slot 0, flags=1): run ~DR then free the object.
            dr->__vftable->dtr_aidr_DR(dr, 1);
        }
    }
}
