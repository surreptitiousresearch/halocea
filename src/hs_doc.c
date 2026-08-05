/* hs_doc @0x83730238 — dump the signature and documentation of every HaloScript function (471 entries) to
 * "hs_doc.txt".
 *
 * DEVIATION: the binary copies each documentation string with an inline byte loop; reproduced as strcpy. */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "headers/hs_function_definition.h"
#include "headers/blam_data_globals.h"

extern void hs_get_function_parameters_string(int16_t function_index, char *buffer);

int hs_doc(void)
{
    FILE *file = fopen("hs_doc.txt", "w");
    char buffer[2096];
    for ( int i = 0; i < 471; i = (int16_t)(i + 1) )
    {
        hs_get_function_parameters_string(i, buffer);
        fprintf(file, "%s\r\n", buffer);
        strcpy(buffer, hs_function_table[i]->documentation);
        fprintf(file, "%s\r\n\r\n", buffer);
    }
    return fclose(file);
}
