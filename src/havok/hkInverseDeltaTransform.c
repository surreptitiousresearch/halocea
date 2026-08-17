/* hkInverseDeltaTransform @0x83FAC1B8 */
/* hkInverseDeltaTransform — undo a delta (successive-difference) encoding in
   place: each sample becomes the running cumulative sum of itself and all
   preceding deltas. data[0] is the seed; data[i] += data[i-1] for i >= 1.
   DEVIATION: the original unrolls the prefix sum by four (with a scalar
   remainder loop); reproduced as a single running-total loop, which is
   arithmetically identical. */
void hkInverseDeltaTransform(float *deltaData, unsigned int nValues)
{
    float runningTotal = deltaData[0];
    unsigned int i;

    for (i = 1; i < nValues; ++i)
    {
        runningTotal = deltaData[i] + runningTotal;
        deltaData[i] = runningTotal;
    }
}
