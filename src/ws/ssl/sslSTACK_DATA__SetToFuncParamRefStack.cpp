/* ?SetToFuncParamRefStack@sslSTACK_DATA@@QAAAAV1@PAVsslSTACK@@H@Z @0x82AF0888 */
#include "sslSTACK_DATA.h"

// 0x82AF0888 — sslSTACK_DATA::SetToFuncParamRefStack. Stores a (stack, index) function-parameter
// reference into the funcParamStack arm.
sslSTACK_DATA &sslSTACK_DATA::SetToFuncParamRefStack(sslSTACK *pStack, int idx)
{
    Cleanup();
    funcParamStack.pStack = pStack;
    funcParamStack.idx = idx;
    type = TYPE_SSLFUNC_PARAM_REF_STACK;
    return *this;
}
