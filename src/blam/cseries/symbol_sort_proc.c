/* symbol_sort_proc @0x83810C88 */
int symbol_sort_proc(unsigned int *elem1, unsigned int *elem2)
{
    unsigned int key1 = elem1[1];

    if (!key1)
        return 1;

    unsigned int key2 = elem2[1];

    if (key1 > key2)
        return 1;
    if (key2 && key1 >= key2)
        return 0;
    return -1;
}
