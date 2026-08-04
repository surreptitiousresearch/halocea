#include "animMNG.h"
#include "animINST.h"

// animMNG::animMNG(animINST*, animSYSTEM*) @ 0x8267E9B8
// Link this manager to its owning instance/system, wire up the vtable, default-construct the
// two blend-layer vectors (empty, tagged with their allocation call-site for leak diagnostics),
// and default-construct the action-frame list.
// D:\Projects\code\common\src.sys\animation\anim_ctrl.cpp:85 is the source line the decompile
// tags both dsVECTOR __cl allocation cookies with.
animMNG::animMNG(animINST *_pInst, animSYSTEM *animSys)
{
    this->pASys = animSys;
    this->pInst = _pInst;
    this->__vftable = (animMNG_vtbl *)&animMNG::vftable;

    this->layerList.pData = nullptr;
    this->layerList.nElem = 0;
    this->layerList.allocated = 0;
    this->layerList.__cl.file = "D:\\Projects\\code\\common\\src.sys\\animation\\anim_ctrl.cpp";
    this->layerList.__cl.line = 85;

    this->listLayerVisitExcluded.pData = nullptr;
    this->listLayerVisitExcluded.nElem = 0;
    this->listLayerVisitExcluded.allocated = 0;
    this->listLayerVisitExcluded.__cl.file = "D:\\Projects\\code\\common\\src.sys\\animation\\anim_ctrl.cpp";
    this->listLayerVisitExcluded.__cl.line = 85;

    // this->afList is default-constructed implicitly as a member (matches the decompile's
    // explicit animAF_LIST::animAF_LIST(&this->afList) call, which is ordinary compiler-
    // generated member construction -- see animAF_LIST::animAF_LIST for its body).
}
