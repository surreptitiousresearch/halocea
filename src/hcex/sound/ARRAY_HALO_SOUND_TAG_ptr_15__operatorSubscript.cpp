#include "../../headers/ws/ds/dsARRAY.h"
#include "../../headers/hcex/HALO_SOUND_TAG.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;   // ?IGNORE_STRONG_ASSERT@@3HA
extern char empty_string;         // shared assert message-context byte

// ds::ARRAY<const HALO_SOUND_TAG*,15>::operator[] (non-const) @ 0x836B5C48
// Bounds-checked mutable access into the fixed 15-slot tag-pointer list. Uses the const-list
// STRONG_ASSERT_DUMMY::Crash path (ds_const_list.h) rather than the ds_vector asserd helper.
// Explicit member specialization: overrides the trivial inline in dsARRAY.h for this instantiation.
template<>
const HALO_SOUND_TAG *&ds::ARRAY<const HALO_SOUND_TAG *, 15>::operator[](int i)
{
    if (!IGNORE_STRONG_ASSERT && (unsigned)i > 0xE)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr,
            "index >= 0 && index < Size",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
            115,
            (unsigned char)empty_string);
    return this->list[i];
}
