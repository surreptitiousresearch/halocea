/* EncryptBuffer @0x83688790 — in-place TEA (Tiny Encryption Algorithm) block cipher over a buffer,
 * 32 Feistel rounds per 8-byte block using a 128-bit key (4 dwords), with the standard TEA delta
 * constant (0x9E3779B9, accumulated via repeated subtraction of 1640531527 == -0x61C88647 mod 2^32).
 * Any trailing partial-8-byte tail is encrypted separately by decoding its last two dwords in place. */

void EncryptBuffer(int bufferSizeInBytes, char *buffer, int *key)
{
    if ( bufferSizeInBytes < 8 )
        return;

    int block_count = bufferSizeInBytes / 8;
    if ( block_count > 0 )
    {
        char *block = buffer - 4;
        do
        {
            unsigned int block_left = ((unsigned int *)block)[1];
            unsigned int block_right = ((unsigned int *)block)[2];
            int sum = 0;

            for ( int round = 32; round; --round )
            {
                sum -= 1640531527;
                block_left += (16 * block_right + key[0]) ^ ((block_right >> 5) + key[1]) ^ (sum + block_right);
                block_right += ((block_left >> 5) + key[3]) ^ (16 * block_left + key[2]) ^ (sum + block_left);
            }

            ((unsigned int *)block)[1] = block_left;
            block += 8;
            *(unsigned int *)block = block_right;
            --block_count;
        }
        while ( block_count );
    }

    if ( bufferSizeInBytes % 8 )
    {
        char *tail = &buffer[bufferSizeInBytes];
        unsigned int block_left = *((unsigned int *)tail - 2);
        unsigned int block_right = *((unsigned int *)tail - 1);
        int sum = 0;

        for ( int round = 32; round; --round )
        {
            sum -= 1640531527;
            block_left += (16 * block_right + key[0]) ^ ((block_right >> 5) + key[1]) ^ (sum + block_right);
            block_right += ((block_left >> 5) + key[3]) ^ (16 * block_left + key[2]) ^ (sum + block_left);
        }

        char *tail_block = tail - 8;
        *(unsigned int *)tail_block = block_left;
        ((unsigned int *)tail_block)[1] = block_right;
    }
}
