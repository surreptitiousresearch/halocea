#include "animAF_LIST.h"

// animAF_LIST::animAF_LIST() @ 0x82C189A8
// Clear the AF state, default-construct the state-sequence vector (empty), reset every one of
// the 31 short-list slots to "no sequence" (seqID=-1, extID=-1, actionFrameID cleared), then
// default-construct the events flag list and pre-reserve it to 16 entries.
// D:\Projects\code\common\src.sys\animation\anim_af.cpp:14 tags actionFrameStateSeq's allocation
// cookie; d:\projects\code\common\incl.sys\ds\ds_strid_flags.h:13 tags events.propList's.
// CAVEAT: the disasm walks afShortList via a raw pointer advanced by sizeof(animAF)==12 per
// iteration and clears the trailing byte through a reinterpreted-as-dsCONST_ARRAY `nEntry` field
// (a decompiler HIBYTE artifact of the pointer-walk, not a real nEntry write) -- reconstructed
// here as the equivalent indexed loop setting each slot's seqID=-1/extID=-1/actionFrameID=0.
animAF_LIST::animAF_LIST()
{
    this->stateAF.state = 0;

    this->actionFrameStateSeq.pData = nullptr;
    this->actionFrameStateSeq.nElem = 0;
    this->actionFrameStateSeq.allocated = 0;
    this->actionFrameStateSeq.__cl.file = "D:\\Projects\\code\\common\\src.sys\\animation\\anim_af.cpp";
    this->actionFrameStateSeq.__cl.line = 14;

    this->afShortList.nEntry = 0;
    for (int slot = 0; slot < 31; ++slot)
    {
        this->afShortList.list[slot].seqNmb.seqID = -1;
        this->afShortList.list[slot].seqNmb.extID = -1;
        this->afShortList.list[slot].actionFrameID = 0;
    }

    this->events.propList.pData = nullptr;
    this->events.propList.nElem = 0;
    this->events.propList.allocated = 0;
    this->events.propList.__cl.file = "d:\\projects\\code\\common\\incl.sys\\ds\\ds_strid_flags.h";
    this->events.propList.__cl.line = 13;

    this->events.propList.Realloc(16);
}
