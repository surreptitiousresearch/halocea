/* ?SelectMaterialsByInstAffixes@MTL_DESC_DB@@QAAXPAVobjOBJ@@ABV?$dsTSTRING@D@@@Z @0x82A83B38 */
#include "MTL_DESC_DB.h"
#include "../obj/objOBJ.h"

// MTL_DESC_DB::SelectMaterialsByInstAffixes(objOBJ *, const dsTSTRING<char> &) — 0x82A83B38
// ABI: r3 = this, r4 = pObj, r5 = instAffixes (no sret — void return).
//
// Public entry point: thin wrapper recursing over the whole child tree. If `pObj` itself has
// faces, its own materials are selected via the private worker; then every child (objOBJ::child
// is a circular sibling list terminated by looping back to the head, via objOBJ::next) is
// recursed into with the same instance affix string.
void MTL_DESC_DB::SelectMaterialsByInstAffixes(objOBJ *pObj, const dsTSTRING<char> &instAffixes)
{
    if (!pObj)
        return;

    if (pObj->nFace > 0)
        this->_SelectMaterialsByInstAffixes(pObj, instAffixes);

    objOBJ *firstChild = pObj->child;
    bool visitedFirst = false;
    for (objOBJ *child = firstChild; firstChild && (!visitedFirst || child != firstChild); child = child->next)
    {
        this->SelectMaterialsByInstAffixes(child, instAffixes);
        visitedFirst = true;
    }
}
