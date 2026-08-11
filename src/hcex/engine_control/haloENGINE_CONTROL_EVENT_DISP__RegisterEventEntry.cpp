#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::RegisterEventEntry @ 0x823CC478 — construct
// one (id, handler) pair and InsertSorted it into GetEventMap() (INS_DUP_IGNORE: a duplicate id
// keeps the first registration).
// DEVIATION: the DB's raw decompile shows this taking 14 parameters (a13/handler duplicated,
// several unused ints) — disassembly-verified (r3/r4 only feed the dsPAIR construction; every
// other decompiler-invented parameter is dead) to be the real 2-argument form the DB prototype
// already gives: RegisterEventEntry(int id, handler).
void haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::RegisterEventEntry(
    int id, void (haloENGINE_CONTROL::*handler)(unsigned long, const dsPARAM_LIST &))
{
    dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)> entry;
    entry.key = id;
    entry.val = handler;
    GetEventMap()->InsertSorted(entry, dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8>::INS_DUP_IGNORE);
}
