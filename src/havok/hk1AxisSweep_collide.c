#include "../headers/havok/hk1AxisSweep.h"

/* hk1AxisSweep::collide — brute-force interval-overlap sweep-and-prune. Two
   overloads share the flattened name; both live here.
 *
 * The sweep axis is lane 0: boxes are visited in ascending m_min[0] order and a
 * candidate is only tested while its m_min[0] is below the fixed box's m_max[0].
 * The remaining two lanes (1 and 2) are tested for interval overlap; a pair that
 * overlaps on both is emitted as hkKeyPair{keyA, keyB}, where each key is the leaf
 * index packed in m_min[3]. When pairsOut is full the pair is dropped and the
 * overflow counter (numPairsSkipped) is bumped so the caller can re-run with a
 * larger buffer. The return value is the number of pairs written.
 *
 * DEVIATION: the database code is 4-wide unrolled (it advances the scan pointer by
 * four 32-byte records per iteration and carries a redundant all-four-separated
 * early-out), relying on the caller's four trailing sentinel AABBs to terminate the
 * final partial batch. It is rewritten here as the equivalent scalar sweep with
 * clean AabbInt field accesses; the scan's `m_min[0] < fixed.m_max[0]` guard makes
 * the sentinels unnecessary while producing the identical pair set and order. The
 * separation test keeps the original sign-bit form: (a - b) has bit 31 set iff
 * a < b for these quantised lane values, so a box pair overlaps on lanes 1..2 iff
 * the OR of the four boundary differences is non-negative.
 *
 * numPairsSkipped is hkPadSpu<int>* in the true ABI (m_storage); hkPadSpu<int> is
 * layout-identical to int, so it is taken as int* to keep the source clean. */

static int hk1AxisSweep_overlaps12(const hk1AxisSweep_AabbInt *a,
                                   const hk1AxisSweep_AabbInt *b)
{
    unsigned int separated =
        (a->m_max[1] - b->m_min[1]) |
        (b->m_max[1] - a->m_min[1]) |
        (a->m_max[2] - b->m_min[2]) |
        (b->m_max[2] - a->m_min[2]);
    return (separated & 0x80000000u) == 0;
}

/* hk1AxisSweep::collide — single-array self-collision. */
int hk1AxisSweep_collide(hk1AxisSweep_AabbInt *aabbs, int numAabbs,
                         hkKeyPair *pairsOut, int pairsCapacity,
                         int *numPairsSkipped)
{
    hkKeyPair *out = pairsOut;
    hkKeyPair *outEnd = pairsOut + pairsCapacity;
    int i;

    *numPairsSkipped = 0;

    for (i = 0; i < numAabbs - 1; ++i)
    {
        const hk1AxisSweep_AabbInt *fixed = &aabbs[i];
        unsigned int fixedMax0 = fixed->m_max[0];
        const hk1AxisSweep_AabbInt *scan;

        for (scan = &aabbs[i + 1]; scan->m_min[0] < fixedMax0; ++scan)
        {
            if (!hk1AxisSweep_overlaps12(fixed, scan))
                continue;
            if (out >= outEnd)
                ++*numPairsSkipped;
            else
            {
                out->m_keyA = fixed->m_min[3];
                out->m_keyB = scan->m_min[3];
                ++out;
            }
        }
    }

    return (int)(out - pairsOut);
}

/* hk1AxisSweep::collide — two-array overload (flattened name disambiguated). keyA
   always comes from the pa array, keyB from the pb array. */
int hk1AxisSweep_collidePair(hk1AxisSweep_AabbInt *pa, int numA,
                             hk1AxisSweep_AabbInt *pb, int numB,
                             hkKeyPair *pairsOut, int pairsCapacity,
                             int *numPairsSkipped)
{
    hkKeyPair *out = pairsOut;
    hkKeyPair *outEnd = pairsOut + pairsCapacity;

    *numPairsSkipped = 0;

    while (numA > 0 && numB > 0)
    {
        const hk1AxisSweep_AabbInt *scan;

        if (pa->m_min[0] <= pb->m_min[0])
        {
            /* Fixed pa; scan pb forward while it can still overlap on lane 0. */
            unsigned int fixedMax0 = pa->m_max[0];
            for (scan = pb; scan->m_min[0] < fixedMax0; ++scan)
            {
                if (!hk1AxisSweep_overlaps12(pa, scan))
                    continue;
                if (out >= outEnd)
                    ++*numPairsSkipped;
                else
                {
                    out->m_keyA = pa->m_min[3];
                    out->m_keyB = scan->m_min[3];
                    ++out;
                }
            }
            ++pa;
            --numA;
        }
        else
        {
            /* Fixed pb; scan pa forward. */
            unsigned int fixedMax0 = pb->m_max[0];
            for (scan = pa; scan->m_min[0] < fixedMax0; ++scan)
            {
                if (!hk1AxisSweep_overlaps12(scan, pb))
                    continue;
                if (out >= outEnd)
                    ++*numPairsSkipped;
                else
                {
                    out->m_keyA = scan->m_min[3];
                    out->m_keyB = pb->m_min[3];
                    ++out;
                }
            }
            ++pb;
            --numB;
        }
    }

    return (int)(out - pairsOut);
}
