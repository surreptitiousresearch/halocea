/* ?Delete@psSECTION_BRANCH@@QAAXXZ @0x82519A90 */
#include "psSECTION_BRANCH.h"
#include "psSECTION_DATA.h"
#include <new>
// 0x82519A90 — tear down the branch: flag deleting, recursively clear all parent references,
// destroy and free the top section body, then free the branch object itself.

void psSECTION_BRANCH::Delete()
{
    psSECTION_DATA *topSection = this->pTopSection;
    this->isDeleting = 1; // raw: *((unsigned char *)this + 8) |= 0x40
    this->RecClearParents(topSection);

    psSECTION_DATA *topToFree = this->pTopSection;
    if (topToFree)
    {
        topToFree->~psSECTION_DATA();
        ::operator delete(topToFree);
    }
    ::operator delete(this);
}
