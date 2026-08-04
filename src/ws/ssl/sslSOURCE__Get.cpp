#include "sslSOURCE.h"
// 0x825CAB58  sslSOURCE::Get
// Returns a copy of the source text. As with sslOBJECT::GetDbgName, the decompiler shows the
// sret/this swap: it shares `source`'s buffer (index [1] = offset 4) into the return string and
// bumps the buffer refcount -- the dsTSTRING<char> copy constructor.

dsTSTRING<char> sslSOURCE::Get() const
{
    return source; // copy-ctor: shares source's buffer and increments its refcount
}
