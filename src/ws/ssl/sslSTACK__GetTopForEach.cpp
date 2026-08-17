/* ?GetTopForEach@sslSTACK@@QAAAAUsslFOREACH_DATA@@XZ @0x82AF1EF0 */
#include "sslSTACK.h"
// 0x82AF1EF0  sslSTACK::GetTopForEach  (thunk -> dsVECTOR<sslFOREACH_DATA,8>::Back)

sslFOREACH_DATA &sslSTACK::GetTopForEach()
{
    return foreachStack.Back();
}
