extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

void * pool_alloc(unsigned int size)
{
    return dlMalloc(size, "D:\\Projects\\code\\HCEX\\sources\\interface\\ui_widget.c", 0x7A);
}
