#include "../headers/havok/hkpWorld.h"
#include "../headers/havok/hkpSimulationIsland.h"

typedef struct hkpPhysicsSystem hkpPhysicsSystem;
typedef struct hkpAction hkpAction;

/* boundary — Havok SDK, not reversed */
extern void hkpPhysicsSystem_addAction(hkpPhysicsSystem *self, hkpAction *action); /* hkpPhysicsSystem::addAction(hkpAction *) */

/* Copies every action owned by the world into the physics system: walks both the
   active and inactive simulation-island arrays and, for each island, adds each
   entry of its m_actions array (hkArray<hkpAction *>) to the system. */
void enumerateAllActionsInWorld(hkpWorld *world, hkpPhysicsSystem *physicsSystem)
{
    hkpSimulationIsland **island;
    hkpSimulationIsland **islandsEnd;
    hkpAction **actions;
    int actionIndex;

    /* Active simulation islands */
    island = (hkpSimulationIsland **)world->m_activeSimulationIslands.m_data;
    islandsEnd = island + world->m_activeSimulationIslands.m_size;
    while (island != islandsEnd)
    {
        actions = (hkpAction **)(*island)->m_actions.m_data;
        if ((*island)->m_actions.m_size > 0)
        {
            actionIndex = 0;
            do
            {
                hkpPhysicsSystem_addAction(physicsSystem, actions[actionIndex]);
                ++actionIndex;
            }
            while (actionIndex < (*island)->m_actions.m_size);
        }
        ++island;
    }

    /* Inactive simulation islands */
    island = (hkpSimulationIsland **)world->m_inactiveSimulationIslands.m_data;
    islandsEnd = island + world->m_inactiveSimulationIslands.m_size;
    while (island != islandsEnd)
    {
        actions = (hkpAction **)(*island)->m_actions.m_data;
        if ((*island)->m_actions.m_size > 0)
        {
            actionIndex = 0;
            do
            {
                hkpPhysicsSystem_addAction(physicsSystem, actions[actionIndex]);
                ++actionIndex;
            }
            while (actionIndex < (*island)->m_actions.m_size);
        }
        ++island;
    }
}
