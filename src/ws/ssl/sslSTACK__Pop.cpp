#include "sslSTACK.h"
// 0x82AF1B48  sslSTACK::Pop

void sslSTACK::Pop()
{
    int nElem = mainStack.nElem;
    if ( nElem )
        mainStack.ShrinkResize(nElem - 1);
}
