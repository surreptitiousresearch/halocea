// apMEM_FRAME_DUMPER::Dump @ 0x82C4B758 — private variadic log forwarder.
// Passes the caller's format label and argument list straight to apForceLogV.
//
// CAVEAT: this is a variadic passthrough. The decompiler exploded the "..." into a
// dozen phantom integer/quad parameters and hand-built the va_list by spilling
// r6..r10 to the stack home area (disasm 82C4B760..770). The real signature per the
// mangle (?Dump@...@@AAAXPBDPADZZ) is (const char* name, char* format, ...); the
// register spills are just the standard variadic home-area setup, reconstructed here
// as an ordinary va_start/va_end pair. `this` is unused by the body.
#include "apMEM_FRAME_DUMPER.h"
#include <stdarg.h>

void apMEM_FRAME_DUMPER::Dump(const char *name, char *format, ...)
{
    va_list args;
    va_start(args, format);
    apForceLogV(name, format, (char *)args);
    va_end(args);
}
