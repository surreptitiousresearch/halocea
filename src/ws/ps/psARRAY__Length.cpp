/* ?Length@psARRAY@@QBAHXZ @0x827AE838 */
#include "psARRAY.h"
#include "psARRAY_DATA.h"
// 0x827AE838 — element count, or 0 when the handle is null.

int psARRAY::Length() const
{
    psARRAY_DATA *arrayBody = this->pData;
    if (arrayBody)
        return arrayBody->data.nElem;
    return 0;
}
