#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"

// entENTITY::OnRegister @ 0x8252F910
// Registration hook: adopt the caller-supplied multiplayer unique id, if any.
void entENTITY::OnRegister(const entCREATE_INFO &info)
{
    if (info.mpID != (unsigned int)-1)
        SetUID(info.mpID);
}
