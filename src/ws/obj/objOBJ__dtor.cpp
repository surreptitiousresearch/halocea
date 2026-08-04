#include "objOBJ.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// objOBJ::~objOBJ() @ 0x82662138 (protected — only objOBJ and derived classes destroy directly)
// Delete every child (Delete() detaches+destroys each, unlinking it from this->child), detach
// this node from its own parent/sibling list, tear down this object's owned data (geometry,
// name, modifiers, ...), destroy the embedded property section, then drop this object's affix
// string buffer reference.
objOBJ::~objOBJ()
{
    for (objOBJ *nextChild = this->child; nextChild; nextChild = this->child)
        objOBJ::Delete(nextChild);
    this->Detach();
    this->_DestroyData();
    this->ps.~psSECTION();

    dsTSTRING_BUF_HEADER<char> *affixBuffer = this->affixes.str.pBuffer;
    if (affixBuffer->refCount-- == 1)
        dlFree(affixBuffer);
}
