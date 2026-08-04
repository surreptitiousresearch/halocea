// aiPLANNER::DebugCreateAIEntity @ 0x83195858
// ?DebugCreateAIEntity@aiPLANNER@@IAAXABV?$dsTSTRING@D@@0@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_DebugCreate_boundary.h" // entCreate, entCREATE_DATA, gsCameraGet, gsGetPlayer
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/cdt/cdtINFO.h"
#include "../../headers/ws/cdt/cdtREFINE.h"
#include "../../headers/ws/gs/gsSR_MANAGER.h"                  // pSrMng, srMANAGER::ShootRay
#include "../../headers/ws/gs/gsLVL_SYSTEM_boundary.h"         // gsScenePtr
#include "../../headers/ws/ent/entENTITY.h"                    // ent->pInst
#include "../../headers/animINST.h"                            // cdtLayer / ClearIGNORE_CDT_ALWAYS
#include "../../headers/apCL.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"                 // _m3dLineCombine

#include <string.h> // strncpy

// dsSPrintf — format into an existing dsTSTRING<char> (pointer-out form). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// Monotonic debug-unit counter (DB global `i`, referenced as i++ at the spawn site).
static int g_dbgAiUnitCounter = 0;

// Debug helper: ray-cast forward from the current camera, and if it hits geometry, create an AI
// entity of class `nameClass` (optional template `nameTpl`) oriented to the hit surface, tagged
// with a unique "dbg_ai_unit_N" name and forced onto collision layer 1.
void aiPLANNER::DebugCreateAIEntity(const dsTSTRING<char> &nameClass, const dsTSTRING<char> &nameTpl)
{
    if (!nameClass.pBuffer->strLen)
        return;

    // Ray origin = camera world position (matrC2W translation row); direction = camera forward
    // (matrC2W basis row at elements[8..10]).
    camCAMERA *cam = gsCameraGet(0);
    m3dV from;
    from.x = cam->matrC2W.elements[12];
    from.y = cam->matrC2W.elements[13];
    from.z = cam->matrC2W.elements[14];

    cam = gsCameraGet(0);
    m3dV dir;
    dir.x = cam->matrC2W.elements[8];
    dir.y = cam->matrC2W.elements[9];
    dir.z = cam->matrC2W.elements[10];

    cdtINFO info;
    entENTITY *player = gsGetPlayer(0);
    // Refine: exclude the player's own instance; layer mask 0x20.
    cdtREFINE refine(0, 32, player->pInst);

    apCL cl = { "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_debug.cpp", 367 };
    pSrMng->ShootRay(from, dir, 500.0f, &refine, &info, cl); // return ignored; validity read below

    if (info.IsValid())
    {
        entCREATE_DATA createData;

        dsTSTRING<char> name;
        dsSPrintf(&name, "dbg_ai_unit_%d", g_dbgAiUnitCounter++);
        // Copy the generated name into a fixed 64-char buffer (create-data instance name slot).
        char instName[64];
        strncpy(instName, name.pBuffer->str, 0x3F);

        m3dV pos = *info.GetVClos();
        m3dV normal;
        info.GetFaceNormal(&normal);
        // Nudge the spawn point 1cm off the surface along its normal.
        // DEVIATION: the decompiler miscounted _m3dLineCombine as 5 args (spurious slot); the real
        // ABI is (base, dir, t, out) with out == &pos.
        _m3dLineCombine(&pos, &normal, 0.0099999998f, &pos);

        m3dMATR matr;
        matr._MakeLCS2WCS_VY(&pos, &normal);

        const char *tpl = (nameTpl.pBuffer->strLen > 0) ? nameTpl.pBuffer->str : nullptr;
        entENTITY *ent = entCreate(gsScenePtr, nameClass.pBuffer->str, tpl, &createData, nullptr);
        if (ent)
        {
            animINST *inst = ent->pInst;
            if (!inst->cdtLayer)
            {
                inst->cdtLayer = 1;
                inst->ClearIGNORE_CDT_ALWAYS();
            }
        }
        // name (dsTSTRING) and createData (entCREATE_DATA) destructors run here.
    }
}
