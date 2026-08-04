#include "animCREATE_DATA.h"

// animCREATE_DATA::ApplySRData @ 0x8252BF08
// If this creation block carries shader-replacement data, copy it into the caller's record.
void animCREATE_DATA::ApplySRData(rendSR_DATA *_srData)
{
    rendSR_DATA *srData = this->srData;
    if (srData)
        _srData->Copy(*srData); // Copy takes const rendSR_DATA& (deref the pointer)
}
