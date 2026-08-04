#include "psARRAY.h"
#include <cstddef>
#include "psARRAY_DATA.h"
#include "../ds/dsDATA.h"
#include "../../headers/apCL.h"
// 0x827AFC78 — append `data` to the array, lazily allocating and initialising the ref-counted
// backing store (an empty element vector, refCount 1) on first use.

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line);

void psARRAY::PushBack(const dsDATA &data)
{
    if (!this->pData) {
        psARRAY_DATA *body = static_cast<psARRAY_DATA *>(
            operator new(sizeof(psARRAY_DATA),
                         "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp", 0xB5u));
        if (body) {
            // inlined psARRAY_DATA construction: empty dsVECTOR<dsDATA,8> element store + refCount 1
            apCL cl;
            cl.file = "D:\\Projects\\code\\common\\src.sys\\ps\\ps_array.cpp";
            cl.line = 363;
            body->data.pData = nullptr;
            body->data.nElem = 0;
            body->data.allocated = 0;
            body->data.__cl = cl;
            body->refCount = 1;
        }
        this->pData = body;
    }
    this->pData->data.PushBack(data);
}
