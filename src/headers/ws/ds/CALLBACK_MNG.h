#pragma once
#include "dsVECTOR.h"
// ws-engine ds: callback registration list. DB-verified layout (types_members
// ds::CALLBACK_MNG<void (__cdecl*)(void)>): callbacks@0 (dsVECTOR<T,8>, 20B) — size 20.

namespace ds {

template<class T>
struct CALLBACK_MNG {
    dsVECTOR<T, 8> callbacks; /* 0x00 */

    // Invoke every registered callback in order (reversed per-instantiation; see
    // CALLBACK_MNG_void__ProcessAll.cpp). Declared so that explicit specialization matches.
    void ProcessAll();
};

} // namespace ds
