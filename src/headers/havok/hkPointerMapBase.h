#pragma once
#include "hkThreadMemory.h"

/* hkPointerMapBase<KEY,VALUE,Ops> — concrete layout of the open-addressing pointer
 * hash map that backs hkPointerMap<K,V>. DB-verified (types_members): the base is
 * 12 bytes — Pair *m_elem @0, int m_numElems @4, int m_hashMod @8. The template's
 * KEY/VALUE always resolve to unsigned long in this binary, so the Pair holds two
 * 32-bit words. m_numElems carries a DONT_DEALLOCATE flag in its sign bit
 * (0x80000000): when negative the storage is caller-owned and not freed. */

typedef struct hkPointerMapPair
{
    unsigned int key;   /* 0x00 — -1 (0xFFFFFFFF) marks an empty slot */
    unsigned int val;   /* 0x04 */
} hkPointerMapPair;

typedef struct hkPointerMapBase
{
    hkPointerMapPair *m_elem;  /* 0x00 */
    int               m_numElems; /* 0x04 — sign bit = DONT_DEALLOCATE */
    int               m_hashMod;  /* 0x08 — (slotCount - 1), power-of-two mask */
} hkPointerMapBase;

void hkPointerMapBase_construct(hkPointerMapBase *self, hkPointerMapPair *ptr, unsigned int sizeInBytes); /* hkPointerMapBase::hkPointerMapBase(ptr,size) */
void hkPointerMapBase_construct_default(hkPointerMapBase *self);          /* hkPointerMapBase::hkPointerMapBase() */
void hkPointerMapBase_destruct(hkPointerMapBase *self);                     /* hkPointerMapBase::~hkPointerMapBase */
int  hkPointerMapBase_insert(hkPointerMapBase *self, unsigned int key, unsigned int val); /* hkPointerMapBase::insert */
unsigned int hkPointerMapBase_findKey(hkPointerMapBase *self, unsigned int key);          /* hkPointerMapBase::findKey */

/* Deeper callees kept as boundary externs (2-level descent limit). */
extern void hkPointerMapBase_resizeTable(hkPointerMapBase *self, int newSlotCount); /* hkPointerMapBase::resizeTable */
extern int  hkPointerMapBase_remove(hkPointerMapBase *self, unsigned int key);      /* hkPointerMapBase::remove */
