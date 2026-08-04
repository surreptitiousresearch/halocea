#include "psSECTION_INTERFACE.h"
#include <cstddef>
#include <new> // placement new (body) psSECTION_DATA()
#include "psSECTION_DATA.h"
// 0x8251A218 — release any prior body then allocate and default-construct a fresh empty
// psSECTION_DATA into this handle (leaving it null if the allocation fails).

// Placement/debug global allocator the ws build links in: operator new(size, file, line).
extern void *operator new(size_t size, const char *file, unsigned int line);

void psSECTION_INTERFACE::CreateEmpty()
{
    if (this->pData)
        this->pData->Release();

    psSECTION_DATA *body = static_cast<psSECTION_DATA *>(
        operator new(sizeof(psSECTION_DATA),
                     "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp", 0xE2u));
    if (body) {
        new (body) psSECTION_DATA();
        this->pData = body;
    } else {
        this->pData = nullptr;
    }
}
