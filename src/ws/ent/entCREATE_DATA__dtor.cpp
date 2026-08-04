#include "../../headers/entCREATE_DATA.h"
#include "../../headers/ws/anim/animCREATE_DATA.h"

// entCREATE_DATA::~entCREATE_DATA @ 0x823CE690 — thunk. entCREATE_DATA adds only the pDomSpawn
// pointer (no owned resource), so its destructor merely runs the animCREATE_DATA base destructor.
// The compiler emits this as a direct call to ~animCREATE_DATA (shared body).
entCREATE_DATA::~entCREATE_DATA()
{
    // base animCREATE_DATA::~animCREATE_DATA() runs implicitly.
}
