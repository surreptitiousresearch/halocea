/* enumerateAllPhantomsInWorld @0x838F7AE0 */
#include "../headers/havok/hkpWorld.h"
#include "../headers/havok/hkpBroadPhaseBorder.h"
#include "../headers/havok/hkpPhantom.h"

typedef struct hkpPhysicsSystem hkpPhysicsSystem;

/* boundary — Havok SDK, not reversed */
extern void hkpPhysicsSystem_addPhantom(hkpPhysicsSystem *self, hkpPhantom *phantom); /* hkpPhysicsSystem::addPhantom(hkpPhantom *) */

/* Copies every phantom registered in the world into the physics system, skipping
   the six broad-phase border phantoms (they belong to the world, not the system). */
void enumerateAllPhantomsInWorld(hkpWorld *world, hkpPhysicsSystem *physicsSystem)
{
    hkArray<hkpPhantom *> *phantoms;
    hkpBroadPhaseBorder *border;
    int phantomCount;
    int index;

    phantoms = &world->m_phantoms;
    border = world->m_broadPhaseBorder;
    phantomCount = phantoms->m_size;

    if (border)
    {
        if (phantomCount > 0)
        {
            index = 0;
            do
            {
                hkpPhantom *phantom = ((hkpPhantom **)phantoms->m_data)[index];
                if (phantom != border->m_phantoms[0]
                    && phantom != border->m_phantoms[1]
                    && phantom != border->m_phantoms[2]
                    && phantom != border->m_phantoms[3]
                    && phantom != border->m_phantoms[4]
                    && phantom != border->m_phantoms[5])
                {
                    hkpPhysicsSystem_addPhantom(physicsSystem, phantom);
                }
                ++index;
            }
            while (index < phantoms->m_size);
        }
    }
    else
    {
        if (phantomCount > 0)
        {
            index = 0;
            do
            {
                hkpPhysicsSystem_addPhantom(physicsSystem, ((hkpPhantom **)phantoms->m_data)[index]);
                ++index;
            }
            while (index < phantoms->m_size);
        }
    }
}
