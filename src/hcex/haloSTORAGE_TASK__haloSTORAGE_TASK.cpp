#include "../headers/hcex/haloSTORAGE_TASK.h"

// @0x823F4BF0 — forwards to the sysUSER_STORAGE_TASK base ctor, then resets error/isReady to 0.
// DEVIATION: the decompiler shows `u` re-acquired into a fresh local REF_TYPE (bumping its
// refcount) before the base-ctor call, then Release()'d on `u` afterward -- this is exactly the
// compiler's by-value-parameter passing convention for a non-trivial-copy type (ds_data::REF_TYPE
// has a real copy ctor/dtor per its own header), not extra logic of this constructor; passing `u`
// by value below reproduces it without hand-rolling the refcount dance.
haloSTORAGE_TASK::haloSTORAGE_TASK(ds_data::REF_TYPE<sysUSER> u, TASK_TYPE t, sysUSER_CONTENT_TYPE ct, bool async)
    : sysUSER_STORAGE_TASK(u, t, ct, async)
{
    error = 0;
    isReady = false;
}
