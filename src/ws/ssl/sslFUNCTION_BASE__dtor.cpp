#include "sslFUNCTION_BASE.h"
#include "sslFUNCTION_DECL.h"

// sslFUNCTION_BASE::~sslFUNCTION_BASE() @ 0x82AD8418
// When stateFunc bit 0x1 is set (this element privately owns pDecl -- see header caveat), free
// the declaration. `delete pDecl` compiles down to exactly the decompile's explicit sequence
// (param vector dtor, then operator delete) since sslFUNCTION_DECL has no other non-trivial
// member and no user-declared destructor of its own.
sslFUNCTION_BASE::~sslFUNCTION_BASE()
{
    if ((this->stateFunc & 1) != 0)
    {
        sslFUNCTION_DECL *pDecl = this->pDecl;
        if (pDecl)
            delete pDecl;
    }
}
