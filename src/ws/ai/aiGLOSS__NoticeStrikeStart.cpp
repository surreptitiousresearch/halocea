#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

// 0x8323AAB8 — aiGLOSS::NoticeStrikeStart. For a SUID-sparsed strike, mark the (SUID, shooter-team)
// record as currently playing (creating it if absent). `vars` is unused by the body.
void aiGLOSS::NoticeStrikeStart(const aiS_VARS &vars, const wpnS_DESC &desc, aiBRAIN *shooter)
{
    (void)vars;
    if ( desc.typeSparse == wpnS_DESC::SPARSE_BY_SUID )
    {
        propGAME_INFO_TEAM *team = shooter->GetTeamID();
        FindCreateSparsing(desc.SUID, team->team)->isPlaying = true;
    }
}
