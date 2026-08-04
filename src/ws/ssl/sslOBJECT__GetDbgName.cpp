#include "sslOBJECT.h"
// 0x825246E0  sslOBJECT::GetDbgName
// Returns a copy of the debug name. The decompiler renders the sret return buffer as `this` and
// the real `this` (sslOBJECT) as `result` (sret/this swap): it stores dbgName.pBuffer into the
// return string and bumps the buffer refcount -- i.e. the dsTSTRING<char> copy constructor sharing
// the buffer. Expressed here as the idiomatic by-value member copy.

dsTSTRING<char> sslOBJECT::GetDbgName() const
{
    return dbgName; // copy-ctor: shares dbgName's buffer and increments its refcount
}
