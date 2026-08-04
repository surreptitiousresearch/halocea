#include "sslSTACK.h"
// 0x82AF1ED0  sslSTACK::FreeTopForeach

void sslSTACK::FreeTopForeach()
{
    int nElem = foreachStack.nElem;
    if ( nElem )
        foreachStack.ShrinkResize(nElem - 1);
}
