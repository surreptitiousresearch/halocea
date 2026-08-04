void bit_byte_position_from_absolute_bit_position(unsigned int pos,
                                                    unsigned int *byte_number,
                                                    unsigned int *bit_in_byte)
{
    *bit_in_byte  = pos & 7;
    *byte_number  = pos >> 3;
}
