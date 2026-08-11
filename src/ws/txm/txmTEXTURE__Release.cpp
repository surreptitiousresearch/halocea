#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern int osLockedDecrement(int *pVar);   // boundary -- interlocked decrement (?osLockedDecrement@@YAHPAH@Z)

/* 0x823EF360 -- txmTEXTURE::Release()
 *
 * Interlocked-decrements the texture's reference count. In debug builds (IGNORE_STRONG_ASSERT
 * clear) it crashes if the count ever goes below the -1 "already released" sentinel. Returns
 * non-zero once the count has reached zero or below, signalling the caller should free/unload
 * the texture. */
unsigned int txmTEXTURE::Release()
{
    int *refCountPtr = &this->refCount;

    osLockedDecrement(&this->refCount);

    if ( !IGNORE_STRONG_ASSERT && *refCountPtr < -1 )
    {
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "refCount >= -1",
            "d:\\projects\\code\\common\\incl.sys\\drv\\txm.h",
            247,
            empty_string);
    }

    // Disasm uses `orc r9,r11,r10(=-r11)` i.e. refCount | ~(-refCount), which is algebraically
    // refCount | (refCount - 1); the sign bit (>>31) is 1 exactly when refCount <= 0 (just
    // dropped to zero, or already sitting at the -1 "released" sentinel).
    return (unsigned int)(*refCountPtr | (*refCountPtr - 1)) >> 31;
}
