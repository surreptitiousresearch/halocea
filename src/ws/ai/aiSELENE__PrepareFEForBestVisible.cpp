#include "../../headers/ws/ai/aiSELENE.h"
#include "../../headers/ws/ai/aiPLANNER.h"       // aiPLANNER (owns the fe filter block)
#include "../../headers/ws/ai/aiCON_CB_globals.h" // extern aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiFILT_ENEMY.h"

// aiSELENE::PrepareFEForBestVisible() @ 0x832B3A18
// ?PrepareFEForBestVisible@aiSELENE@@MAAXXZ
//
// (Re)build the planner's shared enemy filter for this brain and narrow it to the shootable,
// potentially-visible candidates sorted by distance — the working set from which the best visible
// enemy is later selected.
void aiSELENE::PrepareFEForBestVisible()
{
    // aiPLANNER::fe is a byte-exact aiFILT_ENEMY block (DB layout carries it as raw bytes).
    aiFILT_ENEMY *fe = (&aiPlanner->fe);
    fe->Start(pBrain);
    fe->PassAllyOpp(pBrain);
    fe->PassShootable(pBrain);
    fe->PassEnemiesCount(pBrain);
    fe->PassPotentiallyVisible(pBrain);
    fe->PassDistance(pBrain);
}
