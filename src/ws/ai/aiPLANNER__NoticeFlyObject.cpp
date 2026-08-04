#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_frontier.h"
#include <string.h>

// aiPLANNER::NoticeFlyObject(aiBRAIN*, entENTITY*, const m3dV&, const m3dV&, const m3dOBB&, float)
//   — 0x83164ED8 (virtual)
// A fast object (grenade/projectile) is flying past. If it is moving fast enough (>= 3 u/s),
// sweep its bounding box forward along `dir` and tell every eligible AI unit whose body the swept
// trajectory reaches. Below 3 u/s the object is ignored.
void aiPLANNER::NoticeFlyObject(aiBRAIN *shooter, entENTITY *obj, const m3dV &from,
                                const m3dV &dir, const m3dOBB &obb, float speed)
{
    (void)obj;
    if (speed < 3.0f)
        return;

    // Forward displacement applied to each OBB corner: dir * speed * 3.
    m3dV disp;
    disp.x = (float)((float)(dir.x * speed) * 3.0f);
    disp.y = (float)((float)(dir.y * speed) * 3.0f);
    disp.z = (float)((float)(dir.z * speed) * 3.0f);

    m3dV verts[8];
    obb.GetVertices(verts);
    for (int i = 0; i < 8; ++i)
    {
        verts[i].x = verts[i].x + disp.x;
        verts[i].y = verts[i].y + disp.y;
        verts[i].z = verts[i].z + disp.z;
    }

    // Expanded, swept trajectory box enclosing the object's start and displaced corners.
    m3dOBB trajObb;
    memcpy(&trajObb, &obb, sizeof(trajObb));
    trajObb.Expand(2.5f);
    trajObb.CalcUpdate(verts, 8);

    int nElem = wah.arrUnits.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = wah.PeekActive(i);
        aiBRAIN *brain = watcher->brain.pHandle ? watcher->brain.pHandle->pPtr : nullptr;

        if (brain && aiBRAIN_ShouldNoticeFlyObject(brain) && brain != shooter)
        {
            aiBRAIN_DbgSetFlyObj(brain, &obb, &trajObb);
            if (IsWithinBox(&trajObb, brain))
                aiMIND_NoticeFlyObject(aiBRAIN_Mind(brain), &from, &dir);
        }
    }
}
