#include "sslSYSTEM.h"
// @0x825CD610  sslSYSTEM::Log(const dsTSTRING<char> &)  [overload __2: higher addr than the
// const char* overload at 0x825CC5F0]. Forwards to the C-string Log with the buffer's characters.

void sslSYSTEM::Log(const dsTSTRING<char> &str)
{
    Log(str.pBuffer->str);
}
