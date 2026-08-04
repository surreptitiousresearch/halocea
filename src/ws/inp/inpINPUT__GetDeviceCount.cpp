#include "../../headers/ws/inp/inpINPUT.h"

// 0x823C2380 — thin forwarder onto the device list's element count.
int inpINPUT::GetDeviceCount() const
{
    return deviceList.Length();
}
