/* ?At@sslSTACK@@QAAAAVsslSTACK_DATA@@H@Z @0x82AF1B40 */
#include "sslSTACK.h"
// 0x82AF1B40  sslSTACK::At  (thunk -> dsVECTOR<sslSTACK_DATA,8>::operator[])

sslSTACK_DATA &sslSTACK::At(int idx)
{
    return mainStack[idx];
}
