#include "../../headers/ws/strm/strmRESOURCE_PACK_S3D.h"
#include "../../headers/ws/strm/strmPACK_MANAGER.h"
#include "../../headers/ws/strm/strmSTREAMING_MANAGER.h"
#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h" // template arg strmRESOURCE_TEXTURE

// strmRESOURCE_PACK_S3D<strmRESOURCE,strmRESOURCE_TEXTURE>::AddQuery @ 0x82670D20
// When pack-reading is disabled, clear idRec and go straight to strmSTREAMING_MANAGER::AddQuery.
// When it's enabled, try to resolve this resource against the registered packs
// (strmPACK_MANAGER::CanOpenResourceFile); a miss leaves `pack` null and bails out (returns
// false) WITHOUT enqueueing the load. On a hit, look up the directory record
// (strmPACK_S3D::FindPakRec) to cache idRec, then hand off to strmSTREAMING_MANAGER::AddQuery.
template<>
bool strmRESOURCE_PACK_S3D<strmRESOURCE, strmRESOURCE_TEXTURE>::AddQuery()
{
    if (!IsReadFromPack()) {
        idRec = 0;
        return strmMng.AddQuery(this);
    }

    bool cached = false;
    // cast: `pack` is a strmPACK_S3D* member; CanOpenResourceFile writes through strmPACK_BASE**
    strmPackMng.CanOpenResourceFile(resourceName, pakId, cached, (strmPACK_BASE **)&pack);
    if (!pack)
        // DEVIATION: the decompiler falls through to `return result;` with result == this->pack
        // (a null strmPACK_S3D* reinterpreted as the pointer-typed return the decompiler guessed);
        // the mangled return type is bool, so this is simply "no query enqueued, report failure".
        return false;

    int recIdx = pack->FindPakRec(resourceName, pakId);
    idRec = pack->recTable[recIdx].idRec;
    return strmMng.AddQuery(this);
}
