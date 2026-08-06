#include "../../headers/ws/snd/BUFFER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// boundary — atomic decrement primitive (D3D/CRT-adjacent os* helper), not reversed here.
extern "C" int osLockedDecrement(int *value);

// 0x82696C48 (snd::BUFFER::Release, D:\Projects\code\common\src.sys\drv\sound\snd.cpp:80).
// Drops one reference on this buffer handle. Asserts the count never went negative before this
// call, then atomically decrements refCount. Does not free/dispose the buffer itself — that is
// left to whatever owns the pool once the count reaches zero.
void snd::BUFFER::Release()
{
    if (!IGNORE_STRONG_ASSERT && refCount < 0)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash("refCount >= 0",
                                                 "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd.cpp",
                                                 80,
                                                 empty_string);
    osLockedDecrement(&refCount);
}
