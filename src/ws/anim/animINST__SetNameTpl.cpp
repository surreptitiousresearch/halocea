/* ?SetNameTpl@animINST@@QAAHPBD@Z @0x825E1048 */
#include "animINST.h"
#include "anim_boundary.h"

// ?SetNameTpl@animINST@@QAAHPBD@Z  (0x825E1048) — set the instance's template name. Frees any prior
// template name, then duplicates `name`. Returns 1 on success (or a null `name` that clears it), 0 on
// allocation failure.
int animINST::SetNameTpl(const char *name)
{
    dlFree(this->nameTpl);
    this->nameTpl = nullptr;
    if (!name)
        return 1;
    this->nameTpl = dlStrdup(name, "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp", 0xD1u);
    if (this->nameTpl)
        return 1;
    return 0;
}
