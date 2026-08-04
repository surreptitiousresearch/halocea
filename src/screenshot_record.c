#include "headers/bitmap_data.h"
#include "headers/file_reference.h"

extern const char *targa_export(file_reference *file, const bitmap_data *bitmap);

void screenshot_record(bitmap_data *screen, file_reference *file)
{
    targa_export(file, screen);
}
