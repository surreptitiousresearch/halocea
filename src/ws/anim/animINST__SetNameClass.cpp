/* ?SetNameClass@animINST@@QAAHPBD@Z @0x825E0FD0 */
#include "animINST.h"
#include "anim_boundary.h"

// ?SetNameClass@animINST@@QAAHPBD@Z  (0x825E0FD0) — set the instance's class name. Frees any prior
// class name, then duplicates `name`. Returns 1 on success (or a null `name` that clears it), 0 on
// allocation failure.
int animINST::SetNameClass(const char *name)
{
    dlFree(this->nameClass);
    this->nameClass = nullptr;
    if (!name)
        return 1;
    this->nameClass = dlStrdup(name, "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp", 0xC2u);
    if (this->nameClass)
        return 1;
    return 0;
}
