// osLOCK::osLOCK(const char*) @0x823D092C — zero isInited/threadNmb(-1)/locker_file/
// locker_line(-1), adopt `name` (or "Unknown" when null), then finish platform init via Init().
#include "../../headers/ws/os/osLOCK.h"

osLOCK::osLOCK(const char *_name)
{
    isInited = false;
    threadNmb = -1;
    locker_file = nullptr;
    locker_line = -1;
    name = _name ? _name : "Unknown";
    Init();
}
