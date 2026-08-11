#include "sslSTACK.h"

// @0x82AF1AB8 — sslSTACK::GetTop. Reference to the top operand-stack entry, or a shared function-
// local default (TYPE_INT / 0) returned when the operand stack is empty.
sslSTACK_DATA &sslSTACK::GetTop()
{
    if (this->mainStack.nElem)
        return this->mainStack.Back();

    static sslSTACK_DATA emptyDefault; // default ctor → TYPE_INT/0
    return emptyDefault;
}
