#include "fioFILE.h"
#include "fio_boundary.h"
#include <string.h> // memcpy

// fioFILE::ReadDataImpl(void *, int, int) @ 0x825AC7F8 (protected) — the read core.
// Unbuffered streams delegate straight to DoReadData; buffered streams satisfy the
// request from pBuffer, refilling (with a 16-byte overlap) and re-seeking as needed.
// A short read past streamSize latches FIO_ERROR_EOF. Finally, if the stream is
// byte-reordered, the delivered bytes are endian-swapped in place.

void fioFILE::ReadDataImpl(void *data, int size, int reorderGain)
{
    unsigned char *dst = (unsigned char *)data;
    int remaining = size;

    if ( !pBuffer )
    {
        // Decompiler rendered DoReadData as taking only `this`; the slot really takes
        // (dst, size) — disasm passes r4=dst, r5=remaining.
        DoReadData(dst, remaining);
        offset += remaining;
        goto reorder;
    }

    if ( offset + size > streamSize )
    {
        state.val |= FIO_ERROR_EOF; // 0x80
        remaining = streamSize - offset;
    }

    if ( remaining > 0 )
    {
        unsigned char *out = dst;
        unsigned int need = remaining;
        while ( 1 )
        {
            fioFILE_BUFFER *buf = pBuffer;
            unsigned int pos = offset;
            unsigned int bufOfs = buf->bufFileOfs;
            if ( pos >= bufOfs )
            {
                unsigned int bufEnd = buf->bufDataLen + bufOfs;
                if ( pos < bufEnd )
                {
                    unsigned int chunk = bufEnd - pos;
                    if ( need < chunk )
                        chunk = need;
                    memcpy(out, &buf->data[pos - bufOfs], chunk);
                    need -= chunk;
                    out += chunk;
                    offset += chunk;
                }
            }
            if ( !need )
                break;

            // Refill: keep a 16-byte overlap when the cursor is inside the old window.
            fioFILE_BUFFER *refill = pBuffer;
            int keep = 0;
            unsigned int npos = offset;
            unsigned int nbufOfs = refill->bufFileOfs;
            if ( npos >= nbufOfs + 16 && npos <= refill->bufDataLen + nbufOfs )
            {
                keep = 16;
                memcpy(refill->data, (char *)refill + npos - nbufOfs, 16);
            }
            pBuffer->bufFileOfs = offset - keep;

            fioFILE_BUFFER *sized = pBuffer;
            unsigned int fill = streamSize - sized->bufFileOfs;
            if ( sized->bufSizeCur < fill )
                fill = sized->bufSizeCur;
            sized->bufDataLen = fill;

            DoSeek(offset);
            DoReadData(&pBuffer->data[keep], pBuffer->bufDataLen - keep);
        }
    }

reorder:
    if ( (state.val & FIO_OPEN_BYTE_REORDER) != 0 )
        ds::ByteReorder(dst, remaining, reorderGain);
}
