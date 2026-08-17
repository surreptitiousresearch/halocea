/* ?CalcLTM@cdtSKEL@@QAAXPAVanimINST@@@Z @0x82B0B818 */
#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/obj_free_functions.h"
// objCalcLTMGroup is now reversed — src/ws/obj/objCalcLTMGroup.cpp,
// declared in obj_free_functions.h (closes this boundary).

// 0x82B0B818
void cdtSKEL::CalcLTM(animINST *pInst)
{
    objCalcLTMGroup(pInst->pObj);
}
