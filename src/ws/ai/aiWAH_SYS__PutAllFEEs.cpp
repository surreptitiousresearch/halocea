#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiFEE.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::PutAllFEEs(dsVECTOR<aiFEE,8>& arr, IsIncludeFEECheck* checker) @ 0x831728B0
// ?PutAllFEEs@aiWAH_SYS@@QAAXAAV?$dsVECTOR@VaiFEE@@$07@@PAVIsIncludeFEECheck@1@@Z
//
// For every holder, build an aiFEE from it; if the record is valid and (no checker, or the
// checker accepts the holder's watcher), append it to `arr`.
void aiWAH_SYS::PutAllFEEs(dsVECTOR<aiFEE, 8> &arr, IsIncludeFEECheck *checker)
{
    aiFEE fee;

    const int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWTC_HOLDER *holder = storage[i];
        fee.Reset(holder);
        if (fee.IsValid() && (!checker || (*checker)(holder->watcher)))
            arr.PushBack(fee);
    }
}
