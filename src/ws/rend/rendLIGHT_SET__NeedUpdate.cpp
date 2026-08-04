#include "../../headers/ws/rend/rendLIGHT_SET.h"

// rendLIGHT_SET::NeedUpdate @ 0x82ABBDC8
// Latch `_frameUpdNmb` as the last-update frame and report whether it differs from the value
// previously stored — i.e. whether this light set still needs updating this frame. The store
// happens unconditionally, so a second call in the same frame returns false.

int rendLIGHT_SET::NeedUpdate(unsigned int _frameUpdNmb)
{
    unsigned int prev = frameUpdNmb;
    frameUpdNmb = _frameUpdNmb;
    return _frameUpdNmb != prev;
}
