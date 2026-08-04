#include "../../headers/ws/anim/animINST.h"

// rendIsCullAndValidateInstNeeded(animINST*) @ 0x8273C788
// Atomically test-and-set the instance's cull-info validity flag: writes 1 into
// cullInfo.isValid and returns true when the PREVIOUS value was 0 (i.e. this caller
// is the one that must now run the cull/validate pass). osLockedSet returns the prior
// value; the disasm's cntlzw+extrwi (0x8273C7A0) is the "== 0" test that the decompiler
// prints as `osLockedSet(...) == 0`.
extern "C" int osLockedSet(int *pVar, int val); // os subsystem — boundary

int rendIsCullAndValidateInstNeeded(animINST *pInst)
{
    return osLockedSet(&pInst->cullInfo.isValid, 1) == 0;
}
