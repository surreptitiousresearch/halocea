#include "animTRACKER.h"
#include "animINST.h"
#include "objOBJ_boundary.h"
#include "anim_validate_boundary.h"

// ?GetMatrix@animTRACKER@@QBA_NPAVm3dMATR@@@Z  (0x82B2F110, const) — resolve the world transform
// this tracker follows into `out`, dispatching on `type`. Strips scale, then applies the optional
// offset. Returns false for an unknown type.
bool animTRACKER::GetMatrix(m3dMATR *out) const
{
    unsigned int typeIdx = this->type - 1;
    if (typeIdx > TRK_INST_OBJ_POS - 1)
        return false;

    if (typeIdx < 2)   // TRK_POS / TRK_MATRIX
    {
        *out = this->matr;
    }
    else
    {
        // DEVIATION: the compiler's jump table indexes on type-1 (typeIdx); the cases are written
        // against the DB-named TRK_TYPE values of this->type itself for readability (identical range).
        switch (this->type)
        {
        case TRK_INST: // follow the tracked instance
            if (!this->pInst)
                out->Identity();
            else
                this->GetInstMatrix(this->pInst, out);
            break;

        case TRK_INST_OBJ: // follow the tracked object's local-to-world
            if (this->pObj)
            {
                if (this->pInst)
                    objValidateLTM(this->pObj);
                *out = this->pObj->matrLT;
            }
            else
            {
                out->Identity();
            }
            break;

        case TRK_INST_MATRIX: // stored matrix, then the instance transform
            *out = this->matr;
            if (this->pInst)
            {
                m3dMATR instMatr;
                this->GetInstMatrix(this->pInst, &instMatr);
                out->Transform(&instMatr, 2);
            }
            break;

        case TRK_CAMERA: // mirror of the tracked camera
        {
            camCAMERA *pCamera = this->pCamera;
            *out = pCamera->matrC2W;
            out->Scale(-1.0f, 1.0f, 1.0f, 1); // oper=1 (r7); decompiler misread it as 0x82000000
            break;
        }

        case TRK_INST_OBJ_MATRIX: // stored matrix, then object (or instance) transform
            *out = this->matr;
            if (this->pObj)
            {
                if (this->pInst)
                    objValidateLTM(this->pObj);
                out->Transform(&this->pObj->matrLT, 2);
            }
            else if (this->pInst)
            {
                m3dMATR instMatr;
                this->GetInstMatrix(this->pInst, &instMatr);
                out->Transform(&instMatr, 2);
            }
            break;

        case TRK_INST_POS: // build LCS from the instance's up direction at its position
            if (!this->pInst)
            {
                out->Identity();
            }
            else
            {
                m3dV dirUp;
                m3dV pos;
                this->pInst->GetDirUp(&dirUp);
                this->pInst->GetPos(&pos);
                out->_MakeLCS2WCS_VY(&pos, &dirUp);
            }
            break;

        default: // TRK_INST_OBJ_POS: object origin + instance up direction
            if (this->pObj)
            {
                m3dV org;
                m3dV dirUp;
                this->pObj->matrLT.GetOrigin(&org);
                this->pInst->GetDirUp(&dirUp);
                out->_MakeLCS2WCS_VY(&org, &dirUp);
            }
            else
            {
                out->Identity();
            }
            break;
        }
    }

    out->RemoveScale();
    if (this->mOffset)
        out->Transform(this->mOffset, 1);
    return true;
}
