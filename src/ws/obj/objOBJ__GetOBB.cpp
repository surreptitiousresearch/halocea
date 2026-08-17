/* ?GetOBB@objOBJ@@QAAXPAVm3dOBB@@@Z @0x82A719D8 */
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/objGEOM_UNSHARED.h"
#include "../../headers/ws/obj/objGEOM_SHARED.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include <string.h>

// 0x82A719E8  ?GetOBB@objOBJ@@QAAXPAVm3dOBB@@@Z
// Fill *pOBB with this object's oriented bounding box in world space.
void objOBJ::GetOBB(m3dOBB *pOBB)
{
    animINST *pInst = this->pInst;
    if ( pInst )
    {
        // state bits 0x4/0x8 mark an animated (skinned) instance: use the instance's own,
        // already-posed OBB rather than transforming the static geometry box.
        if ( (this->state & 0xC) != 0 )
        {
            m3dOBB *instOBB = pInst->GetOBB(0);
            memcpy(pOBB, instOBB, sizeof(m3dOBB));
        }
        else
        {
            if ( !IGNORE_STRONG_ASSERT )
            {
                if ( this->pGeom
                  || (static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                        "pGeom",
                        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_anim.cpp", 37,
                        empty_string), !IGNORE_STRONG_ASSERT) )
                {
                    if ( !this->pGeom->pSharedGeom )
                        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                            "pGeom->pSharedGeom",
                            "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_anim.cpp", 38,
                            empty_string);
                }
            }
            // Advanced transform: matrLT already carries scale.
            this->pGeom->pSharedGeom->bbox.Transform2OBB_ADV(&this->matrLT, pOBB); // member call; box is `this`
        }
    }
    else
    {
        if ( !IGNORE_STRONG_ASSERT )
        {
            if ( this->pGeom
              || (static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "pGeom",
                    "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_anim.cpp", 22,
                    empty_string), !IGNORE_STRONG_ASSERT) )
            {
                if ( !this->pGeom->pSharedGeom )
                    static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                        "pGeom->pSharedGeom",
                        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_anim.cpp", 23,
                        empty_string);
            }
        }
        this->pGeom->pSharedGeom->bbox.Transform(&this->matrLT, pOBB); // member call; box is `this`
    }
}
