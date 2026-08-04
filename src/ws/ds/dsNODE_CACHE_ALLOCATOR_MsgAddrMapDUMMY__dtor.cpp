#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::~dsNODE_CACHE_ALLOCATOR @ 0x8255C2A0
// Term() then destroy nodeList (mirrors dsNODE_CACHE's own dtor; kept as a distinct override
// because the compiler emits a separate symbol for the derived allocator's dtor even though the
// bodies are identical — matches ds::MAP::~MAP, which inlines this same teardown directly rather
// than calling through this dtor for its embedded `allocator` member).
template<>
dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::~dsNODE_CACHE_ALLOCATOR()
{
    Term();
}
