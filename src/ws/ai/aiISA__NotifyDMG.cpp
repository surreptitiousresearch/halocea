#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/prop/propGAME_INFO_DESC.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiISA::NotifyDMG(const dmgDAMAGE&) @ 0x832A1550
// ?NotifyDMG@aiISA@@UAAXABVdmgDAMAGE@@@Z (virtual)
// When damage reports are enabled and the shooter's team is hostile to ours, raise a team DAMAGE
// alert at the shooter's centre (or, when the shooter is unknown, 5m ahead of our own body).
void aiISA::NotifyDMG(const dmgDAMAGE &dmg)
{
    if (!(this->state.val & AI_ISAST_REPORT_DAMAGE))
        return;

    entENTITY *shooter = dmg.params.entShooter.pHandle
                       ? dmg.params.entShooter.pHandle->pPtr
                       : nullptr;

    // Locate the shooter's propGAME_INFO (directly, or by scanning its container's props/components).
    propGAME_INFO *gameInfo = nullptr;
    if (shooter)
    {
        if (shooter->propContainer.typeId.IsDerivedFrom(propGAME_INFO_DESC::TYPE_ID))
        {
            gameInfo = (propGAME_INFO *)&shooter->propContainer;
        }
        else if (shooter->propContainer.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        {
            for (int i = 0; !gameInfo && i < shooter->propContainer.properties.nElem; ++i)
                gameInfo = shooter->propContainer.properties[i]->GetProperty<propGAME_INFO>();
            for (int i = 0; !gameInfo && i < shooter->propContainer.components.nElem; ++i)
                gameInfo = shooter->propContainer.components[i]->GetProperty<propGAME_INFO>();
        }
    }

    dsSTRID teamShooter;
    if (gameInfo)
        teamShooter.id = gameInfo->teamID.team.id;
    dsSTRID teamMine;
    teamMine.id = this->pBrain->GetTeamID()->team.id;

    if (gsTEAMS::Instance().GetAttitude(teamShooter, teamMine) == gsTEAMS::ATT_ENEMY)
    {
        m3dV pos = m3dVZero;
        if (shooter)
        {
            shooter->GetCenter(&pos);
        }
        else
        {
            aiSTATUS *st = this->pBrain->st.pointee;
            pos = st->cs.posBody + 5.0f * st->cs.dirBody;
        }

        dsTSTRING<char> reason("DAMAGE(");
        entENTITY *shooterEnt = dmg.params.entShooter.pHandle
                              ? dmg.params.entShooter.pHandle->pPtr
                              : nullptr;
        if (shooterEnt)
            reason += shooterEnt->GetName();
        reason.Insert(reason.pBuffer->strLen, ")", -1);

        aiISA::ALERT_PARAMS params = {
            AI_ALERT_DAMAGE,
            pos,
            reason,
            true,  // isAlertTeam
            false, // isUseDelay
        };
        this->Alert(params);
    }
}
