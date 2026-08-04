#include "../../headers/ws/txm/txmMANAGER.h"
#include "../../headers/ws/txm/txmTEX_DESC_DB.h"
#include "../../headers/ws/fnm/fnm.h"

// dl allocator free (releases a dsTSTRING buffer header at zero refcount). boundary.
extern void dlFree(void *ptr);

// Process-wide default-descriptor table singleton. DB symbol is g_TexDescDB (the txmTEX_DESC_DB.h
// extern spells it txmTexDescDB; the DB-verified name is used here to match the reference).
extern txmTEX_DESC_DB g_TexDescDB;

// 0x826FBDD0 -- ?Add@txmMANAGER@@QAAPAVtxmTEXTURE@@PBDHH@Z
// Get-or-create the named texture. If it already exists, only its state flags are updated;
// otherwise the texture is allocated, its descriptor filled from the default-desc table (for a
// procedurally-generated texture, flags&0x20) or from its on-disk ".td" sidecar, and SetDesc is
// applied. Returns nullptr for an empty name, a missing on-disk file (when checkFile is set and
// this isn't a generated/no-fail-add texture), or an allocation failure.
txmTEXTURE *txmMANAGER::Add(const char *name, unsigned int flags, int checkFile)
{
    if (!*name)
        return nullptr;

    txmTEXTURE *tex = this->Find(name);
    if (!tex)
    {
        char findDiscScratch;  // FindDisc's outBuffer -- existence probe only, result unused

        if (checkFile && (flags & 0x20) == 0 && !this->noFailAdd
            && !fnmFindDisc(this->path, name, this->ext, &findDiscScratch))
            return nullptr;

        tex = this->Alloc(name, flags);
        if (!tex)
            return nullptr;

        // Fill the descriptor. The name is wrapped in a temporary dsTSTRING for the lookup; its
        // buffer is released inline afterwards (the temporary's inlined dsTSTRING<char> dtor).
        dsTSTRING<char> nameStr;
        nameStr.pBuffer = nullptr;
        nameStr.UnsafeInit(name, -1, 0);

        if ((flags & 0x20) != 0)
            g_TexDescDB.GetDefaultDesc(&tex->desc, nameStr);
        else
            g_TexDescDB.GetTexDesc(&tex->desc, nameStr);

        if (!--nameStr.pBuffer->refCount)
            dlFree(nameStr.pBuffer);

        tex->SetDesc(tex->desc);
    }

    // Fold in the requested flags. If the texture already carries the "loaded" state bit (0x800000)
    // but the requested sRGB/gamma bit (0x800) disagrees with its current value, clear loaded so the
    // texture is reloaded under the new encoding.
    unsigned int state = tex->state.state;
    if (((state >> 23) & 1) != 0 && (((state >> 11) ^ (flags >> 11)) & 1) != 0)
        tex->state.state &= ~0x800000u;
    tex->state.state |= flags;

    return tex;
}
