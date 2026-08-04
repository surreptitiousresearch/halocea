#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"

// vidDEFRAG_POOL_HCEX_X360::~vidDEFRAG_POOL_HCEX_X360 @ 0x823F1440
// Thunk straight through to the base: no fields of its own to release, so the compiler-emitted
// body is just the implicit call into apDEFRAG_POOL::~apDEFRAG_POOL(this) (reproduced here as an
// empty body — the base destructor still runs automatically as part of C++ object teardown).
vidDEFRAG_POOL_HCEX_X360::~vidDEFRAG_POOL_HCEX_X360()
{
}
