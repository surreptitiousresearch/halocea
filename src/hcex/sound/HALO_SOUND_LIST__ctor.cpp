#include "../../headers/hcex/HALO_SOUND_LIST.h"

// HALO_SOUND_LIST::HALO_SOUND_LIST() @ 0x836B7760
//
// CAVEAT: the raw decompile inlines the local-static-guard body of
// dsTSTRING<char>::UnsafeInitEmpty() directly at each of levelName/fileName's initializations
// (constructing the process-wide shared empty-string singleton on first use, then bumping its
// refcount). Reconstructed here as calls to that existing method, matching the established
// corpus convention (e.g. apCOUNTER_TRACKER::apCOUNTER_TRACKER) -- semantically identical
// without duplicating the guarded-static machinery.
//
// Default-construct an empty list: no memory/loaded flags, levelName/fileName adopt the shared
// empty dsTSTRING singleton, and every dsVECTOR member (tags/streams/streamUsed) starts empty
// with its own allocation call-site cookie (all pointing at ds_vector.h:24 -- the generic
// dsVECTOR(apCL) delegating ctor the compiler inlined here for each member).
HALO_SOUND_LIST::HALO_SOUND_LIST()
{
    this->isMemory = false;
    this->isLoaded = false;

    this->levelName.pBuffer = nullptr;
    this->levelName.UnsafeInitEmpty();

    this->fileName.pBuffer = nullptr;
    this->fileName.UnsafeInitEmpty();

    this->tags.pData = nullptr;
    this->tags.nElem = 0;
    this->tags.allocated = 0;
    this->tags.__cl = apCL{ "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };

    this->soundsCount = 0;
    this->data = nullptr;
    this->dataLength = 0;

    this->streams.pData = nullptr;
    this->streams.nElem = 0;
    this->streams.allocated = 0;
    this->streams.__cl = apCL{ "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };

    this->streamUsed.pData = nullptr;
    this->streamUsed.nElem = 0;
    this->streamUsed.allocated = 0;
    this->streamUsed.__cl = apCL{ "D:\\Projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };
}
