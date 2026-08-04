/* TEA (Tiny Encryption Algorithm) block encode: encrypts the two 32-bit words at
 * *v in place using the 128-bit key at k, 32 rounds, delta = 0x9E3779B9. */
void code(unsigned int *v, int *k)
{
    unsigned int y = v[0];
    unsigned int z = v[1];
    int sum = 0;
    int k0 = k[0];
    int k1 = k[1];
    int k2 = k[2];
    int k3 = k[3];
    int round;

    for (round = 32; round; --round)
    {
        sum -= 1640531527; /* += delta (0x9E3779B9 as signed) */
        y += ((z << 4) + k0) ^ ((z >> 5) + k1) ^ (sum + z);
        z += ((y >> 5) + k3) ^ ((y << 4) + k2) ^ (sum + y);
    }

    v[0] = y;
    v[1] = z;
}
