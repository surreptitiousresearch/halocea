#pragma once
#include "hkPointerMapBase.h"
/* hkPointerMap<KEY, VALUE> — Havok pointer hash map (12 bytes). DB-verified layout
   (types_members hkPointerMap<hkdBreakableBody *,void *> -> hkPointerMapBase<unsigned long,
   unsigned long,hkPointerMapOperations<unsigned long,unsigned long>>: m_elem@0 (Pair *),
   m_numElems@4, m_hashMod@8). m_map reuses the layout-identical hkPointerMapBase so its
   address is usable directly as hkPointerMapBase* (was an anonymous struct). */
template<class KEY, class VALUE>
struct hkPointerMap
{
    hkPointerMapBase m_map;
};
