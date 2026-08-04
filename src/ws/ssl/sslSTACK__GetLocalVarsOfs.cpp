#include "sslSTACK.h"
// 0x82AF0588  sslSTACK::GetLocalVarsOfs  (number of live locals = base offset for a new frame)

int sslSTACK::GetLocalVarsOfs()
{
    return localsStack.nElem;
}
