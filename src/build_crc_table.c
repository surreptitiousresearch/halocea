/* build_crc_table @0x8371BA58 */
void build_crc_table(unsigned int *crc_table)
{
    for ( unsigned int i = 0; i < 256; i++ )
    {
        unsigned int crc = i;
        for ( int bit = 0; bit < 8; bit++ )
        {
            if ( crc & 1 )
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        crc_table[i] = crc;
    }
}
