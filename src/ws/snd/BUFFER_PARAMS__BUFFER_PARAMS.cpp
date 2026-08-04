#include "../../headers/ws/snd/BUFFER_PARAMS.h"

// boundary — zero-vector constant (m3d subsystem global), not reversed here.
extern const m3dV m3dVZero;

// 0x823D64D8 (snd::BUFFER_PARAMS::BUFFER_PARAMS, default ctor).
// Zero-initializes the position/timing/blocking/callback/userData fields, then default-constructs
// `dbgInfo` as an empty string. The DB shows dsTSTRING<char>::UnsafeInitEmpty's lazy-init
// machinery (the function-local-static guard + emptyStr singleton + atexit destructor
// registration) inlined at the call site; modeled here as the equivalent UnsafeInitEmpty call —
// same pattern used elsewhere in this corpus (e.g. apDATA_TRACKER::RECORD::RECORD).
snd::BUFFER_PARAMS::BUFFER_PARAMS()
{
    pos = m3dVZero;
    timeOffset = 0;
    blocking = false;
    callback = nullptr;
    userData = nullptr;
    dbgInfo.pBuffer = nullptr;
    dbgInfo.UnsafeInitEmpty();
}
