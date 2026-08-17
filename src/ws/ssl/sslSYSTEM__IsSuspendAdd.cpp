/* ?IsSuspendAdd@sslSYSTEM@@QBA_NXZ @0x823CDAF0 */
#include "../../headers/ws/ssl/sslSYSTEM.h"

// 0x823CDAF0 — const getter for the "suspend script-object additions" flag.
bool sslSYSTEM::IsSuspendAdd() const
{
    return isSuspendAdd;
}
