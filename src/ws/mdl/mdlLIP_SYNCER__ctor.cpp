// 0x82746F08  ??0mdlLIP_SYNCER@@QAA@XZ  — default constructor.
// Clears all 5 phoneme slots (seq ids = -1, coef = 0, spline = null), empties objList, zeroes the
// timers, sets isClosedOnly=true, useClosedAsIdle=false, pInst=null, sampleRate=44100, status=OFF.
// The compiler-emitted vptr store (mdlLIP_SYNCER vftable) is implicit in this C++ ctor.
#include "../../headers/ws/mdl/mdlLIP_SYNCER.h"
#include "../../headers/apCL.h"

mdlLIP_SYNCER::mdlLIP_SYNCER()
{
    seqList.nEntry = 0;

    // Allocation call-site cookie stamped into the empty objList (matches ds_vector.h:24).
    apCL back_chain = { "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.h", 24 };

    for (int i = 0; i < 5; ++i) {
        seqList.list[i].seq.seqID = -1;
        seqList.list[i].seq.extID = -1;
        seqList.list[i].coef = 0.0f;
        seqList.list[i].spline = nullptr;
    }

    objList.pData = nullptr;
    objList.nElem = 0;
    objList.allocated = 0;
    objList.__cl = back_chain;

    timeEaseCur = 0.0f;
    timeCur = 0.0f;
    isClosedOnly = true;
    timeEnd = 0.0f;
    useClosedAsIdle = false;
    pInst = nullptr;
    sampleRate = 44100;
    status = OFF;
}
