#include "fioFILE.h"
#include "../ds/ds_boundary.h"   // ds::ByteReorder
#include <string.h>             // memcpy

// fioFILE::ReadData(char *, int, int) @ 0x825AC9C8 — public buffered read that caps each
// underlying transfer at 512 KiB (0x80000). Requests up to that size delegate straight to
// ReadDataImpl. Larger requests are split into 512 KiB spans; between spans it pumps
// ProcessBackgroundTasks so streaming I/O keeps flowing, and (when the FIO_REORDER flag,
// bit 5, is set) byte-reorders each completed span in place. A final partial span (< 512 KiB)
// goes through ReadDataImpl.
//
// The unbuffered fast path (pBuffer == null) issues one DoReadData per 512 KiB span; the
// buffered path refills pBuffer with a 16-byte overlap and re-seeks, matching ReadDataImpl.

extern "C" void ProcessBackgroundTasks(double budget); // ws streaming pump — boundary

// signature matches header/mangle (void*); body works on it as bytes
void fioFILE::ReadData(void *_dataArg, int size, int reorderGain)
{
    char *_data = (char *)_dataArg;
    if ( size <= 0x80000 )
    {
        ReadDataImpl(_data, size, reorderGain);
        return;
    }

    char *span = _data;
    char *last_full = &_data[(size / 0x80000) << 19]; // end of the last whole 512 KiB span

    while ( span != last_full )
    {
        int span_len = 0x80000;

        if ( !pBuffer )
        {
            // Unbuffered: one raw span read.
            DoReadData(span, 0x80000);
            offset += 0x80000;
        }
        else
        {
            // Buffered: clamp the final span to the stream tail, latching FIO eof (bit 7).
            if ( offset + 0x80000 > streamSize )
            {
                state.val |= 0x80u;
                span_len = streamSize - offset;
                if ( span_len <= 0 )
                    goto next_span;
            }

            char *dst = span;
            unsigned int remaining = span_len;
            for ( ;; )
            {
                fioFILE_BUFFER *buf = pBuffer;
                unsigned int pos = offset;
                unsigned int bufFileOfs = buf->bufFileOfs;
                if ( pos >= bufFileOfs )
                {
                    unsigned int bufEnd = buf->bufDataLen + bufFileOfs;
                    if ( pos < bufEnd )
                    {
                        size_t chunk = bufEnd - pos;
                        if ( remaining < chunk )
                            chunk = remaining;
                        memcpy(dst, &buf->data[pos - bufFileOfs], chunk);
                        remaining -= chunk;
                        dst += chunk;
                        offset += chunk;
                    }
                }
                if ( !remaining )
                    break;

                // Refill: keep a 16-byte overlap when the new window abuts the old one.
                fioFILE_BUFFER *rb = pBuffer;
                int overlap = 0;
                unsigned int npos = offset;
                unsigned int rbOfs = rb->bufFileOfs;
                if ( npos >= rbOfs + 16 && npos <= rb->bufDataLen + rbOfs )
                {
                    overlap = 16;
                    memcpy(rb->data, (char *)rb + npos - rbOfs, 0x10u);
                }
                pBuffer->bufFileOfs = offset - overlap;

                fioFILE_BUFFER *fb = pBuffer;
                unsigned int len = fb->bufSizeCur;
                unsigned int tail = streamSize - fb->bufFileOfs;
                if ( fb->bufSizeCur >= tail )
                    len = tail;
                fb->bufDataLen = len;

                DoSeek(offset);
                DoReadData(&pBuffer->data[overlap], pBuffer->bufDataLen - overlap);
            }
        }

        // Byte-reorder the completed span if FIO_REORDER (bit 5) is set.
        if ( ((unsigned int)state.val >> 5) & 1 )
            ds::ByteReorder((unsigned char *)span, span_len, reorderGain);

    next_span:
        span += 0x80000;
        ProcessBackgroundTasks(-1.0);
    }

    // Trailing partial span.
    if ( size % 0x80000 )
    {
        ReadDataImpl(span, size % 0x80000, reorderGain);
        ProcessBackgroundTasks(-1.0);
    }
}
