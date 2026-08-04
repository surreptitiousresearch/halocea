#include "sslERROR.h"

// --- other-subsystem boundary (declared, not reversed here) ---
extern void (*sslLogCb)(int channel, const char *text, int flags); // ssl: global log sink (may be null)

// sslERROR::sslERROR(sslERROR_ID id, const dsTSTRING<char> &desc, int tags) @ 0x825CBB00
// Formatted error construct: point desc at the shared empty-string singleton first (same inlined
// UnsafeInitEmpty idiom as the default ctor), then, unless `desc` aliases this->desc (self-ref
// guard reproduced from the decompile), release that placeholder and share `desc`'s buffer
// instead. When `id` denotes a real error (non-zero), forward the message text to the global log
// sink if one is installed.
// NOTE: `tags` is accepted and stored but the decompile never assigns it in this constructor body
// (matches the binary -- tags is left at whatever value operator new / the caller's stack left,
// i.e. effectively uninitialised here); reproduced faithfully rather than guessing a `this->tags =
// tags;` the binary does not perform.
sslERROR::sslERROR(sslERROR_ID id, const dsTSTRING<char> &desc, int tags)
{
    this->desc.pBuffer = nullptr;
    this->desc.UnsafeInitEmpty();

    this->id = id;
    if (&desc != &this->desc)
        this->desc = desc;

    if (this->id)
    {
        if (sslLogCb)
            sslLogCb(1, desc.pBuffer->str, 0);
    }
}
