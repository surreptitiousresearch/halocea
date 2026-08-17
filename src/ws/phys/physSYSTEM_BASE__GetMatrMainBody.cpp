/* ?GetMatrMainBody@physSYSTEM_BASE@@QAAXAAVm3dMATR@@@Z @0x82C40D80 */
#include "../../headers/ws/phys/physSYSTEM_BASE.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// 0x82C40D80
void physSYSTEM_BASE::GetMatrMainBody(m3dMATR &out)
{
    objOBJ *pObjMainBody = this->pObjMainBody;
    if (pObjMainBody)
    {
        // Decompiler unrolled this into 8 qword-sized load/store pairs (disasm: 8x ld/std at
        // matrLT+0x00.. +0x38, one full m3dMATR width) and mangled the addressing of the last
        // three with a bogus HIDWORD() cast — the whole sequence is just a straight 64-byte
        // transform copy. Reconstructed as a plain struct assignment.
        out = pObjMainBody->matrLT;
    }
    else
    {
        // No main rigid body yet — fall back to the anim instance's local-to-world transform.
        pInst->_GetMatrL2W(&out);
    }
}
