/* ?GetPresetName@propINST_CONSTRUCTOR@@QBAABV?$dsTSTRING@D@@H@Z @0x826E19B8 */
#include "../../headers/ws/prop/propINST_CONSTRUCTOR.h"
// 0x826E19B8 — propINST_CONSTRUCTOR::GetPresetName(int) const
// Ignores `this` entirely (same as GetPresetCount): indexes the shared instCONSTRUCTOR_MNG's
// preset list and returns the name of the i-th preset node by reference.
// Deviation from decompiler: the decompiler printed this as a raw pointer reinterpret
// (`(const dsTSTRING<char> *)*dsVECTOR<...>::operator[](&mng.presets, i)`), because
// instCONSTRUCTOR_NODE::objName sits at offset 0 of instCONSTRUCTOR_NODE, so reinterpreting the
// node pointer as a dsTSTRING<char>* is bit-identical to taking objName's address. Written here
// as the equivalent, type-correct field access instead of the raw cast.

const dsTSTRING<char> &propINST_CONSTRUCTOR::GetPresetName(int i) const
{
    return mng.presets[i]->objName;
}
