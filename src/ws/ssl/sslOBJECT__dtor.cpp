#include "sslOBJECT.h"
#include "sslCLASS.h"
#include "sslSYSTEM.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"

// --- other-subsystem boundary (declared, not reversed here) ---
extern "C" void dlFree(void *ptr); // allocator: heap free (dlmalloc-family), same convention as os_boundary.h

// sslOBJECT::~sslOBJECT() @ 0x82A37E28
// Decrement the live-object counter and let the owning system drop this uid from its object
// registry, then destroy every stored member value and free the array: `arrStoredMembers[-1]`
// is the compiler's array-new element-count cookie (read here as the leading int of that slot,
// matching the decompile's `.type`-shaped read of the same 4 bytes) and the eh-vector-destructor
// iterator tears the elements down in reverse order before the whole cookie+array block is freed.
// Each dsDATA's teardown is reproduced as an explicit runtime-type Destroy call (same convention
// as sslSTACK_DATA::ConvertToDsData) since dsDATA has no destructor declared in this codebase.
// Finally release the debug-name and name string buffers (manual ref-count drop, matching the
// dsTSTRING convention used throughout this batch).
//
// CAVEAT: the decompile also shows an explicit `sslCLASS_REF::~sslCLASS_REF(&this->refClass)` call
// sitting between the dbgName and name buffer releases. refClass is a real sslCLASS_REF member
// with its own non-trivial destructor (already reversed/declared), so in this C++ reconstruction
// it is destroyed automatically by the compiler-generated member epilogue after this body returns
// -- adding an explicit call here as well would double-release the class ref. That moves its
// teardown to after (rather than between) the two string-buffer releases; the three operations
// are mutually independent (no shared state), so this reordering has no observable effect.
sslOBJECT::~sslOBJECT()
{
    --__sslNObj;
    this->refClass.pClass->pSslSystem->_OnObjectDelete(this->uid); // member-call form

    dsDATA *arrStoredMembers = this->arrStoredMembers;
    if (arrStoredMembers)
    {
        int count = *(reinterpret_cast<int *>(arrStoredMembers) - 1);
        for (int i = count - 1; i >= 0; --i)
        {
            if (arrStoredMembers[i].type)
                arrStoredMembers[i].type->Destroy(&arrStoredMembers[i].storage);
        }
        operator delete[](reinterpret_cast<char *>(arrStoredMembers) - 4);
    }

    dsTSTRING_BUF_HEADER<char> *dbgNameBuffer = this->dbgName.pBuffer;
    if (dbgNameBuffer->refCount-- == 1)
        dlFree(dbgNameBuffer);

    dsTSTRING_BUF_HEADER<char> *nameBuffer = this->name.pBuffer;
    if (nameBuffer->refCount-- == 1)
        dlFree(nameBuffer);

    // this->refClass is destroyed automatically here (compiler-generated member epilogue) -- see CAVEAT.
}
