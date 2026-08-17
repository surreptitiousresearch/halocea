/* enumerateAllInactiveEntitiesInWorld @0x838F7628 */
#include "../headers/havok/hkpWorld.h"
#include "../headers/havok/hkpSimulationIsland.h"
#include "../headers/havok/hkpWorldObject.h" // m_collidable.base.m_shape (was raw +16 read)
#include "../headers/havok/hkBool.h"

typedef struct hkpPhysicsSystem hkpPhysicsSystem;
typedef struct hkpEntity hkpEntity;
typedef struct hkpRigidBody hkpRigidBody;

/* boundary — Havok SDK, not reversed */
extern void hkpPhysicsSystem_addRigidBody(hkpPhysicsSystem *self, hkpRigidBody *body); /* hkpPhysicsSystem::addRigidBody(hkpRigidBody *) */

/* Copies every "inactive" rigid body in the world into the physics system: the
   entities of the world's fixed island plus the entities of each inactive
   simulation island. The fixed island's first entity is the shared world fixed
   rigid body and is only added when its hkpEntity+16 field is non-zero; every
   other entity is always added. Returns whether the world held any such entities
   (the fixed island had entities, OR there was at least one inactive island).
   Signature reversed from disasm: the decompiler's phantom D3DDevice/uint tail
   args are ABI artifacts; addRigidBody takes only (self, body), and the leading
   pointer is the hidden hkBool return slot. */
hkBool enumerateAllInactiveEntitiesInWorld(hkpWorld *world, hkpPhysicsSystem *physicsSystem)
{
    hkpSimulationIsland *fixedIsland;
    hkpEntity **fixedEntities;
    int fixedEntityCount;
    int fixedEntityIndex;
    char sawFixedEntities;

    hkpSimulationIsland **island;
    hkpSimulationIsland **islandsEnd;
    int inactiveIslandCount;
    hkpEntity **entities;
    int entityIndex;

    hkBool anyEntities;

    sawFixedEntities = 0;

    /* Fixed island: its entities are permanently inactive (static geometry). */
    fixedIsland = world->m_fixedIsland;
    if (fixedIsland)
    {
        fixedEntities = (hkpEntity **)fixedIsland->m_entities.m_data;
        fixedEntityCount = fixedIsland->m_entities.m_size;
        for (fixedEntityIndex = 0; fixedEntityIndex < fixedEntityCount; ++fixedEntityIndex)
        {
            if (fixedEntityIndex != 0
                || ((hkpWorldObject *)fixedEntities[0])->m_collidable.base.m_shape != 0) /* typed: hkpEntity base hkpWorldObject::m_collidable@16, hkpCdBody::m_shape@0 (DB) */
            {
                hkpPhysicsSystem_addRigidBody(physicsSystem, (hkpRigidBody *)fixedEntities[fixedEntityIndex]);
            }
            fixedEntityCount = fixedIsland->m_entities.m_size; /* size re-read each pass, per disasm */
        }
        sawFixedEntities = (fixedEntityCount > 0);
    }

    /* Inactive simulation islands: add every entity of each. */
    inactiveIslandCount = world->m_inactiveSimulationIslands.m_size;
    island = (hkpSimulationIsland **)world->m_inactiveSimulationIslands.m_data;
    islandsEnd = island + inactiveIslandCount;
    if (island != islandsEnd)
    {
        do
        {
            entities = (hkpEntity **)(*island)->m_entities.m_data;
            if ((*island)->m_entities.m_size > 0)
            {
                entityIndex = 0;
                do
                {
                    hkpPhysicsSystem_addRigidBody(physicsSystem, (hkpRigidBody *)entities[entityIndex]);
                    ++entityIndex;
                }
                while (entityIndex < (*island)->m_entities.m_size);
            }
            inactiveIslandCount = world->m_inactiveSimulationIslands.m_size;
            ++island;
        }
        while (island != (hkpSimulationIsland **)world->m_inactiveSimulationIslands.m_data + inactiveIslandCount);
    }

    anyEntities.m_bool = (inactiveIslandCount > 0 || sawFixedEntities) ? 1 : 0;
    return anyEntities;
}
