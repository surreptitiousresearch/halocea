/* ?SetName@animINST@@QAAHPBD@Z @0x825E0F58 */
#include "animINST.h"
#include "anim_boundary.h"

// ?SetName@animINST@@QAAHPBD@Z  (0x825E0F58) — set the instance's display name. Frees any prior
// name, then duplicates `name`. Returns 1 on success (including a null `name` that just clears it),
// 0 if the duplication allocation failed.
int animINST::SetName(const char *name)
{
    dlFree(this->name);
    this->name = nullptr;
    if (!name)
        return 1;
    this->name = dlStrdup(name, "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp", 0xB2u);
    if (this->name)
        return 1;
    return 0;
}
