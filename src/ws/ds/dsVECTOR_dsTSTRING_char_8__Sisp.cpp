#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the string compare is inlined below

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::Sisp<dsCMP,dsSWAP<dsTSTRING<char>>> @ 0x82663888
// Heap sift-down over the 1-based `array` in range [p,q]: pin array[p]'s buffer as the sinking
// root value, then repeatedly compare it against the larger of its two children, moving the larger
// child up into the vacated parent slot (sharing its buffer, releasing the old occupant) and
// descending, until the root belongs at the current slot or there are no more children. The root's
// pinned buffer is finally installed at the resting slot and the pin reference released.
// CAVEAT: the disasm's final placement step guards on a raw stack-address comparison
// (`&<stack temp> != &array[parent]`) that is always true for any real call site — reconstructed
// as an unconditional placement (dropping what reads as decompiler/register-spill noise, not a
// real source-level condition).
template<>
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Sisp<dsCMP, dsSWAP<dsTSTRING<char>>>(
        int p, int q, dsTSTRING<char> *array, const dsCMP &, const dsSWAP<dsTSTRING<char>> &)
{
    dsTSTRING_BUF_HEADER<char> *rootBuffer = array[p].pBuffer;
    ++rootBuffer->refCount; // pin the sinking root value

    int child = 2 * p;
    bool noChild = (2 * p > q);
    if (2 * p < q) {
        // Pick the larger of the two children: array[2p] vs array[2p+1].
        const char *leftChar = array[2 * p].pBuffer->str;
        const char *rightChar = array[2 * p + 1].pBuffer->str;
        int diff;
        do {
            int leftByte = (unsigned char)*leftChar;
            diff = leftByte - (unsigned char)*rightChar;
            if (leftByte == 0)
                break;
            ++leftChar;
            ++rightChar;
        } while (!diff);
        if (diff < 0) // array[2p] < array[2p+1]
            ++child;
        noChild = child > q;
    }

    int parent = p;
    if (!noChild) {
        while (true) {
            const char *rootChar = rootBuffer->str;
            const char *childChar = array[child].pBuffer->str;
            int forwardDiff;
            do {
                int rootByte = (unsigned char)*rootChar;
                forwardDiff = rootByte - (unsigned char)*childChar;
                if (rootByte == 0)
                    break;
                ++rootChar;
                ++childChar;
            } while (!forwardDiff);
            if (forwardDiff >= 0)
                break; // root >= child: heap property satisfied here

            dsTSTRING<char> *parentSlot = &array[parent];
            if (&array[child] != parentSlot) {
                dsTSTRING_BUF_HEADER<char> *oldParentBuffer = parentSlot->pBuffer;
                if (oldParentBuffer->refCount-- == 1)
                    dlFree(oldParentBuffer);
                dsTSTRING_BUF_HEADER<char> *childBuffer = array[child].pBuffer;
                parentSlot->pBuffer = childBuffer;
                ++childBuffer->refCount;
            }

            parent = child;
            child *= 2;
            bool stop = child > q;
            if (child < q) {
                const char *leftChar2 = array[child].pBuffer->str;
                const char *rightChar2 = array[child + 1].pBuffer->str;
                int diff2;
                do {
                    int leftByte2 = (unsigned char)*leftChar2;
                    diff2 = leftByte2 - (unsigned char)*rightChar2;
                    if (leftByte2 == 0)
                        break;
                    ++leftChar2;
                    ++rightChar2;
                } while (!diff2);
                if (diff2 < 0)
                    ++child;
                stop = child > q;
            }
            if (stop)
                break;
        }
    }

    // Install the pinned root value at its resting slot, then release the pin.
    dsTSTRING<char> *finalSlot = &array[parent];
    dsTSTRING_BUF_HEADER<char> *oldFinalBuffer = finalSlot->pBuffer;
    if (oldFinalBuffer->refCount-- == 1)
        dlFree(oldFinalBuffer);
    finalSlot->pBuffer = rootBuffer;
    ++rootBuffer->refCount;

    if (rootBuffer->refCount-- == 1)
        dlFree(rootBuffer);
}
