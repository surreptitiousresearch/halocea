/* bit_byte_position_from_absolute_bit_position @0x8380F7A8 */
/* DEVIATION: out-params were swapped vs the binary — r4 receives pos&7 (bit_in_byte:
 * 0x8380F7B0 stw r11,0(r4)) and r5 receives pos>>3 (byte_number: 0x8380F7B4 stw r10,0(r5)). */
void bit_byte_position_from_absolute_bit_position(unsigned int pos,
                                                    unsigned int *bit_in_byte,
                                                    unsigned int *byte_number)
{
    *bit_in_byte  = pos & 7;
    *byte_number  = pos >> 3;
}
