#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/obj_free_functions.h"
// objCalcLTMGroupExcl is now reversed — src/ws/obj/objCalcLTMGroupExcl.cpp,
// declared in obj_free_functions.h (closes this boundary).

// 0x82B0B810
void cdtSKEL::CalcLTMExcl(animINST *pInst)
{
    objCalcLTMGroupExcl(pInst->pObj);
}
