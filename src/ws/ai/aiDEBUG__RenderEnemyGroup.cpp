// aiDEBUG::RenderEnemyGroup @ 0x83194C98
// ?RenderEnemyGroup@aiDEBUG@@IAAXXZ
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"   // rendDrv, dbg_isEn* con-vars, m3dVUnitX/Zero, m3dMatrIdentity, nearModel
#include "../../headers/ws/ai/aiBRAIN.h"              // pBrain->st / mind / GetPos / GetDirUp / GetMinHeightFlesh
#include "../../headers/ws/ai/aiMIND.h"               // mind.enemy
#include "../../headers/ws/ai/aiSTATUS.h"             // st fields (cs / enemy / perc)
#include "../../headers/ws/ai/aiENEMY_BASE.h"         // Is / IsZero / GetInst / GetPeeker
#include "../../headers/ws/ai/aiWATCHER.h"            // GetBodyCS
#include "../../headers/animINST.h"                   // GetMatrL2W
#include "../../headers/ws/rend/rendDRIVER.h"         // RenderLine / RenderCross / RenderCS
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// m3dNormalize / m3dLengthVector / m3d free operators come from aiDEBUG_boundaries.h.
extern const m3dV m3dVUnitZ;                          // ?m3dVUnitZ@@3Um3dV@@A — (0,0,1)

// Draw the per-brain enemy-tracking debug group, gated by per-feature con-vars: thrown-object
// trajectory, enemy coordinate-system gizmo, watcher body CS, dist-to-enemy line, enemy height
// ladder, last-visible marker, last-successful-visibility probe cloud, reticle projection, and the
// enemy "quarter-volume" box. Skipped entirely when there is no valid, non-zero enemy.
void aiDEBUG::RenderEnemyGroup()
{
    aiENEMY_BASE *enemy = this->pBrain->mind.enemy.pointee;
    if (!enemy || enemy->Is(nullptr) || enemy->IsZero())
        return;

    aiSTATUS *st = this->pBrain->st.pointee;

    if (dbg_isEnThrownObj.value)
    {
        m3dV end = this->posThrowObjStart + this->dirThrowObj * 10.0f;
        rendDrv->RenderLine(&this->posThrowObjStart, &end, 0xFF0000FF);
        // DEVIATION: cross colour register lost by the decompiler; use the active debug tint.
        rendDrv->RenderCross(&this->posThrowObjClosest, 0.2f, this->Color);
    }

    if (dbg_isEnCSInst.value && !enemy->Is(nullptr))
    {
        m3dMATR matrL2W = m3dMatrIdentity;
        animINST *inst = enemy->GetInst();
        inst->GetMatrL2W(&matrL2W);
        rendDrv->RenderCS(&matrL2W, 0.2f);
    }

    if (dbg_isEnCSFromWatcher.value)
    {
        aiWATCHER *watcher = enemy->GetPeeker();
        if (watcher)
        {
            m3dV pos, dir, up;
            watcher->GetBodyCS(&pos, &dir, &up);
            m3dV dirEnd = pos + dir;
            m3dV upEnd  = pos + up;
            rendDrv->RenderLine(&pos, &dirEnd, 0xFF0000FF);
            rendDrv->RenderLine(&pos, &upEnd, 0xFFFF0000);
        }
    }

    if (dbg_isEnDistTo.value)
    {
        m3dV selfFace  = st->cs.posFace;
        m3dV enemyFace = st->enemy.cs.posFace;
        // DEVIATION: cross colour registers lost by the decompiler; use the active debug tint.
        rendDrv->RenderCross(&selfFace, 0.2f, this->Color);
        rendDrv->RenderLine(&selfFace, &enemyFace, 0xFF4083FF);
        rendDrv->RenderCross(&enemyFace, 0.1f, this->Color);
    }

    if (dbg_isEnHeight.value)
    {
        m3dV feet  = st->enemy.cs.posBody;
        m3dV up    = st->enemy.cs.dirUpBody;
        m3dV right = m3dVUnitX;
        m3dNormalize(&right);
        m3dNormalize(&up);

        if (nearModel)
        {
            // DEVIATION: the decompiler lost one operand vector; disasm (0x83194FC8..0x83195008)
            // shows the adjustment is 0.5*nearModel[0] + 0.5*m3dVUnitZ applied to the feet anchor.
            feet += (*nearModel) * 0.5f + m3dVUnitZ * 0.5f;
        }

        float heightMinFlesh = st->enemy.heightMinFlesh;
        float height         = st->enemy.height;

        // Bottom cross-bar at the feet.
        m3dV a = feet - right * 0.30000001f;
        m3dV b = feet + right * 0.30000001f;
        rendDrv->RenderLine(&a, &b, 0xFFFF0000);

        // Riser up to the min-flesh height, with a cross-bar.
        m3dV minFlesh = feet + up * heightMinFlesh;
        rendDrv->RenderLine(&feet, &minFlesh, 0xFFFF0000);
        a = minFlesh - right * 0.15000001f;
        b = minFlesh + right * 0.15000001f;
        rendDrv->RenderLine(&a, &b, 0xFFFF0000);

        // Riser up to the full body height, with a cross-bar.
        m3dV top = feet + up * height;
        rendDrv->RenderLine(&minFlesh, &top, 0xFFFF0000);
        a = top - right * 0.30000001f;
        b = top + right * 0.30000001f;
        rendDrv->RenderLine(&a, &b, 0xFFFF0000);
    }

    if (dbg_isEnLastVisible.value)
    {
        m3dV posVisLast = st->enemy.posVisLast;
        rendDrv->RenderCross(&posVisLast, 0.5f, this->Color);
    }

    if (dbg_isEnLstVisTstSccss.value)
    {
        m3dV posRandom = st->enemy.visLastSuccess.points.posRandom;
        rendDrv->RenderCross(&posRandom, 0.5f, this->Color);

        for (int i = 0; i < st->enemy.visLastSuccess.pointsFree.nEntry; ++i)
        {
            m3dV pt = st->enemy.visLastSuccess.pointsFree[i];
            rendDrv->RenderCross(&pt, 0.5f, this->Color);
        }

        m3dV avg = st->enemy.visLastSuccess.posFreeAverage;
        rendDrv->RenderCross(&avg, 0.5f, this->Color);
    }

    if (dbg_isEnReticle.value)
    {
        float minHeightFlesh = this->pBrain->GetMinHeightFlesh();
        m3dV  dirUp = this->pBrain->GetDirUp();
        m3dV  pos   = this->pBrain->GetPos();
        m3dV  eye   = pos + dirUp * minHeightFlesh;
        m3dV  reticle = eye + st->perc.dirToReticle;

        rendDrv->RenderCross(&reticle, 0.1f, 0xFF550000);
        // DEVIATION: cross colour register lost by the decompiler; use the active debug tint.
        rendDrv->RenderCross(&eye, 0.1f, this->Color);

        float distToReticle = st->perc.distToReticle;
        float t = distToReticle / m3dLengthVector(&st->perc.dirToReticle);
        if (distToReticle > 14.999f)
            t = 1.0f;

        m3dV mid = eye * (1.0f - t) + reticle * t;
        rendDrv->RenderLine(&mid, &eye, 0xFF5555FF);
        rendDrv->RenderLine(&reticle, &mid, 0xFFFF55FF);
    }

    if (dbg_isEnVolumeQuarter.value)
    {
        // DEVIATION: the decompiler lost the quarter-volume basis vectors and read the descriptor
        // as a raw float array. Disasm (0x8319551C..0x83195584) recovers the exact operands: the
        // box corner is st.cs.posInst offset by the enemy quarter-volume extents (aiBRAIN+0x54
        // shared-ptr pointee, floats at +0x318/+0x31C/+0x320) along the instance CS axes.
        const float *quarterVol = *reinterpret_cast<const float *const *>(
            reinterpret_cast<const char *>(this->pBrain) + 0x54); // aiBRAIN::spDesc.pointee
        aiSTATUS *stv = this->pBrain->st.pointee;

        m3dV corner = stv->cs.posInst
                    + stv->cs.dirInst      * quarterVol[0xC6]   // +0x318
                    + stv->cs.dirUpInst    * quarterVol[0xC7]   // +0x31C
                    + stv->cs.dirRightInst * quarterVol[0xC8];  // +0x320
        rendDrv->RenderLine(&corner, &stv->enemy.posNav, 0xFF00FF00);

        // DEVIATION: the three box-edge axis operands were lost by the decompiler; disasm shows
        // three length-2 edges — reconstructed structurally along the instance CS axes.
        m3dV e1 = corner + stv->cs.dirInst      * 2.0f;
        m3dV e2 = corner + stv->cs.dirUpInst    * 2.0f;
        m3dV e3 = corner + stv->cs.dirRightInst * 2.0f;
        rendDrv->RenderLine(&corner, &e1, 0xFFFFFFFF);
        rendDrv->RenderLine(&corner, &e2, 0xFFFFFFFF);
        rendDrv->RenderLine(&corner, &e3, 0xFFFFFFFF);
    }
}
