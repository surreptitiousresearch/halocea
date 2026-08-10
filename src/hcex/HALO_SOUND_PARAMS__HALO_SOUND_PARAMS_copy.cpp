#include "../headers/hcex/HALO_SOUND_PARAMS.h"

// 0x836BFBC0 — ??0HALO_SOUND_PARAMS@@QAA@ABU0@@Z (copy ctor). Copies every scalar/dsSTRID/
// dsFLAGS field, then deep-copies the `replace` vector: the new vector starts empty (adopting
// only the source's allocation call-site cookie) and Inserts the full source range at index 0.
HALO_SOUND_PARAMS::HALO_SOUND_PARAMS(const HALO_SOUND_PARAMS &that)
{
    this->tag.id = that.tag.id;
    this->state.val = that.state.val;
    this->volume = that.volume;
    this->volume2D = that.volume2D;
    this->distGt = that.distGt;
    this->distGtTag.id = that.distGtTag.id;
    this->distLt = that.distLt;
    this->distLtTag.id = that.distLtTag.id;
    this->dist2D = that.dist2D;
    this->dist3D = that.dist3D;
    this->mouthTag.id = that.mouthTag.id;
    this->mouthMinDelay = that.mouthMinDelay;
    this->replace.pData = nullptr;
    this->replace.nElem = 0;
    this->replace.allocated = 0;
    this->replace.__cl = that.replace.__cl;
    this->replace.Insert(0, that.replace.pData, that.replace.nElem);
}
