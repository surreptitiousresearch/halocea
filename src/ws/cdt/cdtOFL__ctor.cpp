// cdtOFL::cdtOFL(nID, nID2) @0x827E4F24 — zero the record, allocate idList (nID ushorts); if
// nID2==0, mark state bit 0x1 (single-level mode) and commit nIDMax=nID/nID2Max=0. Otherwise also
// allocate nID2List (nID ushorts), startID2List (nID uints), id2List (nID2 ushorts) — nIDMax/
// nID2Max are only committed once every allocation has succeeded.
#include "../../headers/ws/cdt/cdtOFL.h"

extern "C" void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line);

cdtOFL::cdtOFL(unsigned int nID_, unsigned int nID2_)
{
    state.state = 0;
    nID = 0;
    idList = nullptr;
    nID2List = nullptr;
    startID2List = nullptr;
    nID2 = 0;
    id2List = nullptr;
    nIDMax = 0;
    nID2Max = 0;

    idList = (unsigned short *)dlCalloc(2, nID_, "D:\\Projects\\code\\common\\src.sys\\collis\\cdt_sys.cpp", 0x25);
    if (!idList)
        return;

    if (nID2_ == 0) {
        state.state |= 1;
        nIDMax = nID_;
        nID2Max = 0;
        return;
    }

    nID2List = (unsigned short *)dlCalloc(2, nID_, "D:\\Projects\\code\\common\\src.sys\\collis\\cdt_sys.cpp", 0x2C);
    if (!nID2List)
        return;
    startID2List = (unsigned int *)dlCalloc(4, nID_, "D:\\Projects\\code\\common\\src.sys\\collis\\cdt_sys.cpp", 0x30);
    if (!startID2List)
        return;
    id2List = (unsigned short *)dlCalloc(2, nID2_, "D:\\Projects\\code\\common\\src.sys\\collis\\cdt_sys.cpp", 0x34);
    if (!id2List)
        return;

    nIDMax = nID_;
    nID2Max = nID2_;
}
