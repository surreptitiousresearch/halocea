#include "animLOD_LOD_INFO.h"
#include "animINST.h"

// animLOD::animLOD(animINST*) @ 0x82B0D158
// Link the LOD table to its owning instance and set the default distance thresholds/switch
// state (no LOD selected yet, empty per-level object/distance vectors tagged with their
// allocation call-site). D:\Projects\code\common\src.sys\objects\lod.cpp:132.
animLOD::animLOD(animINST *inst)
{
    this->pInst = inst;
    this->stateLod = 0;
    this->distCamPrev = -100.0f;
    this->distLODAnim = 0.0f;
    this->distLODPauseAnim = 2000000.0f;
    this->distLODOffAnimBlend = 0.0f;
    this->distLODOffLTAnimBlend = 0.0f;
    this->distLODHideInst = 1000.0f;
    this->distLODHideObjInst = 0.0f;
    this->transZone = 0.0f;

    this->lods.pData = nullptr;
    this->lods.nElem = 0;
    this->lods.allocated = 0;
    this->lods.__cl.file = "D:\\Projects\\code\\common\\src.sys\\objects\\lod.cpp";
    this->lods.__cl.line = 132;

    this->lodDist.pData = nullptr;
    this->lodDist.nElem = 0;
    this->lodDist.allocated = 0;
    this->lodDist.__cl.file = "D:\\Projects\\code\\common\\src.sys\\objects\\lod.cpp";
    this->lodDist.__cl.line = 132;

    this->lodFactorCur = 0.0f;
    this->lodNmbCur = -1;
    this->lodNmbScripted = -1;

    // NOTE: distLODOffMatrModel/distLODExtAnim (DB-verified header fields, not touched by this
    // ctor) are left at their implicit default value -- the decompile never assigns them here.
}
