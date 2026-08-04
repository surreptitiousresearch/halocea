#pragma once
#include "hkAabb.h"

/* Boundary types for the hk1AxisSweep sweep-and-prune broadphase used by the
   asset-processing connectivity pass (2-level descent). Not reversed here. */

/* hk1AxisSweep::AabbInt — hkAabbUint32, 32 bytes (DB types_members). Only m_min[0]
   is touched directly (written to 0xFFFFFFFF as an end-of-list sentinel). */
typedef struct hk1AxisSweep_AabbInt
{
    unsigned int m_min[4]; /* 0  (quantised min + expansion/key packing) */
    unsigned int m_max[4]; /* 16 (quantised max + expansion/key packing) */
} hk1AxisSweep_AabbInt;

/* hkKeyPair — a candidate overlapping pair, keys index the leaf array. */
typedef struct hkKeyPair
{
    unsigned int m_keyA; /* 0 */
    unsigned int m_keyB; /* 4 */
} hkKeyPair;

extern void hk1AxisSweep_AabbInt_set(hk1AxisSweep_AabbInt *out, const hkAabb *aabb, int key); /* hk1AxisSweep::AabbInt::set */
extern void hk1AxisSweep_sortAabbs(hk1AxisSweep_AabbInt *aabbs, int numAabbs);                /* hk1AxisSweep::sortAabbs */
extern int  hk1AxisSweep_collide(hk1AxisSweep_AabbInt *aabbs, int numAabbs, hkKeyPair *pairsOut,
                                 int pairsCapacity, int *remainingWork);                       /* hk1AxisSweep::collide (self) */

/* hk1AxisSweep::collide — TWO-array overload. The flattened C name collides with
   the self-collision form above, so the pairwise variant is disambiguated here.
   remainingWork/numPairsSkipped is hkPadSpu<int>* in the true ABI; hkPadSpu<int>
   is layout-identical to int, so it is declared int* to keep the C source clean. */
extern int  hk1AxisSweep_collidePair(hk1AxisSweep_AabbInt *pa, int numA,
                                     hk1AxisSweep_AabbInt *pb, int numB,
                                     hkKeyPair *pairsOut, int pairsCapacity,
                                     int *remainingWork);                                       /* hk1AxisSweep::collide (pair) */
