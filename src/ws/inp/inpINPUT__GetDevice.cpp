#include "../../headers/ws/inp/inpINPUT.h"

// 0x823C9D18 — thin forwarder onto the device list's indexed element access.
inpDEVICE *inpINPUT::GetDevice(int id) const
{
    return deviceList[id];
}
