/* compare_surface_indices @0x837C62D8 — qsort comparator over surface indices. Returns 0 when index1 sorts
 * before index2, else a sign-aware tie/after result. Expression reproduced verbatim from the decompiler. */

int compare_surface_indices(int index1, int index2)
{
    if (index1 < index2)
        return 0;
    /* branchless-signed XOR idiom reduces to signed (index1 > index2); kept as 0/1 comparator result */
    return index1 > index2;
}
