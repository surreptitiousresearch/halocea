/* flush_all_batches @0x837866D0 — flush every UI widget vertex batch (see flush_batch.c).
 *
 * DEVIATION: the decompiler inlined flush_batch's own body (raw pointer walking the 86040-byte
 * widget_batch stride) and rendered the loop's upper bound as `&sound_channels[165]` — a coincidental
 * adjacent global that happens to sit exactly at `&widget_batches[5]` in memory, matching widget_batch.h's
 * own "there are 5 batches" comment. Restored as a call to the already-recovered flush_batch.c. */

extern void flush_batch(int batch_index);

void flush_all_batches(void)
{
    for ( int batch_index = 0; batch_index < 5; ++batch_index )
        flush_batch(batch_index);
}
