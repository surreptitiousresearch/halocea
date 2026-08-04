#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/entCREATE_DATA.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/msg/msgADDR.h"

struct aiBRAIN; // spawned AI unit (RTTI probe only)  boundary

// aiDOM_SPAWN::SpawnSingle @ 0x83264F70  ?SpawnSingle@aiDOM_SPAWN@@EAAXXZ
// Spawn one instance: draw a (class, template) pair from the desc lists (desc[24]=class names,
// desc[17]=template names), build its create-data, and hand it to entCreate. Host-only; no-op when
// the spawn debug switch is set. On success the spawned brain records this domain as its origin.
void aiDOM_SPAWN::SpawnSingle()
{
    if (dbg_isOffSpawn.value)
        return;
    if (!aiSpawn_IsNetHost()) // "IsHost" networked state
        return;

    if (!this->pointsList.nElem)
    {
        _apLog("~Spawn,Error~'%s': can't spawn anything - no points are constructed.",
               this->GetName().pBuffer->str);
        return;
    }

    int idx = this->rxClass.Get();
    dscDESC *desc = this->spDesc.pointee;
    // dsCONST_ARRAY::operator[] returns T& (dsTSTRING<char>&), not a pointer — use '.'
    const char *className =
        (*(dsCONST_ARRAY<dsTSTRING<char>, 20> *)&desc[24])[idx].pBuffer->str;
    const char *tplName =
        (*(dsCONST_ARRAY<dsTSTRING<char>, 20> *)&desc[17])[idx].pBuffer->str;

    entCREATE_DATA cd;
    dsTSTRING<char> classNameStr;
    classNameStr.pBuffer = nullptr;
    classNameStr.UnsafeInit(className, -1, 0);

    // ps for InitCreateData is the desc's embedded parse section (desc[51] region).
    // copy-construct from the desc's embedded section handle (psSECTION has only default/copy/dsDATA ctors)
    psSECTION ps(*(const psSECTION *)&this->spDesc.pointee[51].pBrand);
    this->InitCreateData(cd, ps, classNameStr);

    gsAppState2 |= 0x400u; // mark "spawning in progress" for entCreate
    entENTITY *ent = entCreate(gsScenePtr, className, tplName, &cd, nullptr);
    gsAppState2 &= ~0x400u;

    if (!ent)
    {
        _apLog("~Spawn,Error~'%s': failed Spawn of class '%s' / tpl '%s'",
               this->GetName().pBuffer->str, className, tplName);
        return;
    }

    _apLog("~Spawn~'%s': ok Spawn inst '%s' of class '%s' / tpl '%s'.",
           this->GetName().pBuffer->str, ent->GetName().pBuffer->str, className, tplName);

    if (gsIs<aiBRAIN>(ent))
    {
        // spawned unit is an AI brain: stamp its "spawned from" backref (msgADDR[10].uniqueID).
        ((msgADDR *)ent)[10].uniqueID = (int)this;
    }
    else
    {
        _apLog("~Spawn,Error~'%s': spawned non-AI unit. Check the class name: '%s'",
               this->GetName().pBuffer->str, className);
    }

    ++this->nSpawned;
    this->timeAfterSpawn = 0.0f;
}
