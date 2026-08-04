/* numIndices @ 0x8360B228 — Blam engine.
 * Returns the index count for a primitive run given a stride/topology selector
 * and a triangle count. */

int numIndices(int stride, int numTri)
{
    if (stride == 1)
        return numTri + 2;      /* triangle strip */
    if (stride == 2)
        return 2 * numTri + 1;
    return 4 * numTri;
}
