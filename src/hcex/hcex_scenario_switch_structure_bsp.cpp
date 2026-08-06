/* hcex_scenario_switch_structure_bsp @0x823CEDB0 — set the Blam<->ws world offset for a newly-active
 * structure BSP. Resets hcexWorldOffset to zero, then (if the ws scene is loaded) looks up the scene
 * instance named "structure_bsp_anchor<idx>" and, when present, latches its world position as the offset.
 * scnSCENE / animINST are ws-engine scene-graph boundaries. */

#include "../headers/hcex/hcex_prepare_locations_boundary.h"   /* m3dV, animINST (opaque) */

typedef struct scnSCENE scnSCENE;

extern m3dV       hcexWorldOffset;
extern const m3dV m3dVZero;
extern scnSCENE  *gsScenePtr;
extern animINST  *scnSCENE_FindInstName(scnSCENE *scene, const char *name);
extern void       animINST_GetPos(animINST *inst, m3dV *out);
extern "C" int sprintf_0(char *string, const char *format, ...);

extern "C" void hcex_scenario_switch_structure_bsp(int idx)
{
    hcexWorldOffset = m3dVZero;

    if ( gsScenePtr )
    {
        char instance_name[136];
        sprintf_0(instance_name, "structure_bsp_anchor%d", idx);

        animINST *anchor = scnSCENE_FindInstName(gsScenePtr, instance_name);
        if ( anchor )
            animINST_GetPos(anchor, &hcexWorldOffset);
    }
}
