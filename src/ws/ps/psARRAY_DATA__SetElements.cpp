/* ?SetElements@psARRAY_DATA@@QAAXPBVdsDATA@@H@Z @0x827AFDD8 */
#include "psARRAY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
// 0x827AFDD8 — replace the element storage with `num` copies from `arr`: clear, reserve capacity if
// needed, splice the range in at the front, then shrink the capacity back to the live count.

void psARRAY_DATA::SetElements(const dsDATA *arr, int num)
{
    dsVECTOR<dsDATA, 8> *elements = &this->data;
    elements->Clear();
    if (num > elements->allocated)
        elements->Realloc(num);
    elements->Insert(0, arr, num);
    elements->Realloc(elements->nElem);
}
