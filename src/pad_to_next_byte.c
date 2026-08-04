/* pad_to_next_byte @0x837A0F78 — bit count rounded up to whole bytes. */

int pad_to_next_byte(const int bit_count)
{
    return bit_count / 8 + (bit_count % 8 != 0);
}
