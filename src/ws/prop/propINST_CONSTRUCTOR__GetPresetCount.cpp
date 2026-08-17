/* ?GetPresetCount@propINST_CONSTRUCTOR@@QBAHXZ @0x826DD228 */
#include "../../headers/ws/prop/propINST_CONSTRUCTOR.h"
// 0x826DD228 — propINST_CONSTRUCTOR::GetPresetCount() const
// Ignores `this` entirely: reads the preset count straight off the shared instCONSTRUCTOR_MNG
// singleton (disasm loads &mng directly, never touches r3/this). Every propINST_CONSTRUCTOR
// instance shares the same preset tree.

int propINST_CONSTRUCTOR::GetPresetCount() const
{
    return mng.presets.nElem;
}
