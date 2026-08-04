#include "psARRAY.h"
#include <cstddef>
#include "psARRAY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
#include "../../headers/apCL.h"
// 0x827B01C0 — replace this array's contents with a copy of `data`'s elements. Lazily allocates the
// ref-counted backing store on first use (nothing to do for an empty source with no store), then
// clears, reserves, splices the elements in, and shrinks capacity to the live count.

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug allocator

void psARRAY::SetElements(const dsVECTOR<dsDATA, 8> &data)
{
    if (!this->pData) {
        if (!data.nElem)
            return;

        psARRAY_DATA *newData = (psARRAY_DATA *)operator new(
            sizeof(psARRAY_DATA), "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp", 0x80u);
        if (newData) {
            apCL cl;
            cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp";
            cl.line = 363;
            newData->data.pData = nullptr;
            newData->data.nElem = 0;
            newData->data.allocated = 0;
            newData->data.__cl = cl;
            newData->refCount = 1;
        }
        this->pData = newData;
    }

    dsVECTOR<dsDATA, 8> *dst = &this->pData->data;
    int count = data.nElem;
    dsDATA *src = data.pData;

    dst->Clear();
    if (count > dst->allocated)
        dst->Realloc(count);
    dst->Insert(0, src, count);
    dst->Realloc(dst->nElem);
}
