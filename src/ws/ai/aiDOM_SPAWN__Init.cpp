#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"

// aiDOM_SPAWN::Init @ 0x832642A0  ?Init@aiDOM_SPAWN@@MAAHXZ (virtual)
//
// One-time per-map spawn-domain setup: run the gsDOM_SPAWN base init, reset the scnDOMAIN render
// scratch, load or generate the spawn-point list, seed the three exclusive-random index generators
// (class / affix / behaviour) from the descriptor, then validate the descriptor's spawn counters
// (class list present, nSameType unsupported, nSpawnFirst <= nKillSpawn). Returns 1 on success.

// The spawn descriptor (spDesc.pointee, a dscDESC) is read here as a raw int word-array — matching
// the sibling DoSpawn / IsReadySpawn stride view. Word indices reached:
//   desc[143] = scnDOMAIN render vtable   desc[93]  = class RND range
//   desc[72]  = affix RND range / class-name count (0 => no class names found)
//   desc[114] = behaviour RND range       desc[145] = nSameType (unsupported)
//   desc[144] = nSpawnFirst               desc[142] = nKillSpawn
// The exact field names behind these offsets are a boundary; kept as commented word indices.

// gsDOM_SPAWN base init (returns 0 on failure). boundary.
extern int gsDOM_SPAWN_Init(aiDOM_SPAWN *self);

// aiGetSpawnSys() -> spawn subsystem interface. Only the touched method is modeled. boundary.
struct aiSPAWN_SYS_IFACE { void SetWishToSaveDomPoints(); };
extern aiSPAWN_SYS_IFACE *aiGetSpawnSys();

// Reset the scnDOMAIN render-scratch fields the binary zeroes here (decompiler: pObj=0,
// typeId.id[2]=0, nPoly=2000000.0, scnDOMAIN::__vftable = `renderVtbl`). Those fields live in the
// scnDOMAIN subobject region (0xC8), a boundary in aiDOM_SPAWN.h — routed through a helper rather
// than fabricating offsets. boundary.
extern void scnDOMAIN_ResetSpawnScratch(aiDOM_SPAWN *self, void *renderVtbl);

int aiDOM_SPAWN::Init()
{
    {
        dsTSTRING<char> name = this->GetName();
        _apLog("~Spawn~AI spawn domain '%s': Init started", name.pBuffer->str);
    }

    if (!gsDOM_SPAWN_Init(this))
        return 0;

    int *desc = (int *)this->spDesc.pointee; // dscDESC word-array view

    this->nmbLocks = 0;
    scnDOMAIN_ResetSpawnScratch(this, (void *)desc[143]);

    if (!this->LoadDomainPoints())
    {
        aiSPAWN_SYS_IFACE *spawnSys = aiGetSpawnSys();
        spawnSys->SetWishToSaveDomPoints();
        this->GeneratePointsList();
    }

    this->rxClass.Init(desc[93]);
    this->rxAffix.Init(desc[72]);
    this->rxBhv.Init(desc[114]);

    if (!desc[72]) // no class names
    {
        dsTSTRING<char> name = this->GetName();
        _apLog("~Spawn,Error~'%s': no class names found. Init failed.", name.pBuffer->str);
        return 0;
    }

    if (desc[145] > 0) // nSameType
    {
        dsTSTRING<char> name = this->GetName();
        _apLog("~Spawn,Error~'%s': nSameType param is not supported. Ignoring.", name.pBuffer->str);
    }

    if (desc[144] > desc[142]) // nSpawnFirst > nKillSpawn
    {
        dsTSTRING<char> name = this->GetName();
        _apLog("~Spawn,Error~'%s': nSpawnFirst=%d, but nKillSpawn=%d. Correcting nKillSpawn.",
               name.pBuffer->str, desc[144], desc[142]);
        desc[144] = desc[142];
    }

    {
        dsTSTRING<char> name = this->GetName();
        // DEVIATION: the decompiler passes a scnDOMAIN scratch float (`yHi`) as the "%d points"
        // argument; the semantic value is the constructed spawn-point count.
        _apLog("~Spawn~'%s': Init ended ok, %d points", name.pBuffer->str, this->pointsList.nElem);
    }
    return 1;
}
