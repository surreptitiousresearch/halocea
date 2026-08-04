#include "../headers/havok/hkpCollidable.h"
#include "../headers/havok/hkpPhantom.h"
#include "../headers/havok/hkpWorldObject_BroadPhaseType.h"

/* Recover the owning hkpPhantom from a collidable, if that collidable's broad
   phase handle marks it as a phantom; otherwise null. */
hkpPhantom *hkGetPhantom(const hkpCollidable *collidable)
{
    if (collidable->m_broadPhaseHandle.m_type == BROAD_PHASE_PHANTOM)
        return (hkpPhantom *)((char *)collidable + collidable->m_ownerOffset);
    return 0;
}
