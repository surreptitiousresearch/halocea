/* ?GetPS@animINST@@QBA?AVpsSECTION@@XZ @0x825E18F8 */
#include "animINST.h"
#include "animTPL.h"
#include "../ps/psSECTION.h"
#include "../ps/psSECTION_INTERFACE.h"

// ?GetPS@animINST@@QBA?AVpsSECTION@@XZ  (0x825E18F8) — const (QBA), returns psSECTION BY VALUE (sret).
// The ABI passes the return slot in r3 and `this` in r4; the decompiler swapped their names.
// Return the instance's own section handle, or the source template's section when the instance's
// handle is empty.
psSECTION animINST::GetPS() const
{
    // DB: the ps handle is reinterpreted as a psSECTION_INTERFACE only to test emptiness.
    if (reinterpret_cast<const psSECTION_INTERFACE *>(&this->ps)->IsNull())
        return this->pTpl->ps;   // r11 = this->pTpl (0x15C); copy-construct from &pTpl->ps (0xE4)
    return this->ps;
}
