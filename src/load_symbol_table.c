/* load_symbol_table @0x838112F0 — from D:\Projects\code\HCEX\sources\cseries\stack_walk_windows.c.
 *
 * Parses a linker .MAP file into an in-memory debug_symbol_table used by the stack-walker for
 * address->name resolution. It scans for the "Lib:Object" section header, then reads each symbol
 * line ("<seg>:<offset>  <name>  <rva> <flags> <lib:object>"), interning the symbol name and the
 * library/object string into a growable string pool (deduplicating consecutive identical
 * library/object strings), and appends a debug_symbol record per line. It also handles the
 * "Static symbols" continuation section (reached via the "entry point at" marker). When it sees
 * its own symbol ("_load_symbol_table") it records the map->image load fixup in stack_walk_globals.
 * Finally the table is sorted by rva and trailing zero-rva entries are trimmed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "headers/debug_symbol_table.h"
#include "headers/stack_walk_globals.h"

/* Engine allocator + sibling helpers (already reconstructed) — boundaries here. */
extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern void free_symbol_table(debug_symbol_table *symbol_table);
extern int symbol_sort_proc(unsigned int *elem1, unsigned int *elem2);

int load_symbol_table(char *filename, debug_symbol_table *symbol_table, char *timestamp_str)
{
    FILE *map_file;
    int   header_found;
    unsigned int string_storage_capacity;
    unsigned int string_storage_used;
    unsigned int symbols_capacity;
    char  parse_incomplete;
    char  allocation_failed;
    int   previous_library_object_offset;
    unsigned int symbol_address;
    unsigned int rva_value;
    unsigned int rva_base;
    char *segment_token;
    char *offset_token;
    char *name_token;
    char *rva_token;
    char *library_object_token;
    char *number_end;
    unsigned int name_length;
    unsigned int library_length;
    debug_symbol *new_symbols;
    char         *new_string_storage;
    debug_symbol *new_symbol;
    int   trimmed_count;

    char symbol_name[256];
    char library_object[256];
    char previous_library_object[256];
    char line_buffer[0x4000];

    symbol_table->number_of_symbols = 0;
    symbol_table->string_storage = NULL;
    symbol_table->symbols = NULL;

    map_file = fopen(filename, "r");
    if (!map_file)
        goto finish;

    memset(line_buffer, 0, 0x4000);

    /* Discard the first line, then scan forward for the symbol-section header. */
    header_found = (fgets(line_buffer, 0x4000, map_file) != NULL);
    if (!header_found)
        goto close_file;

    do
    {
        if (fgets(line_buffer, 0x4000, map_file))
        {
            if (strstr(line_buffer, "Lib:Object"))
                break;
            if (strstr(line_buffer, "Timestamp"))
                strstr(line_buffer, timestamp_str); /* result unused in the binary (elided timestamp check) */
        }
        else
        {
            header_found = 0;
        }
    }
    while (header_found);

    if (!header_found)
        goto close_file;

    string_storage_capacity = 0;
    string_storage_used = 0;
    symbols_capacity = 0;
    parse_incomplete = 1;
    allocation_failed = 0;
    previous_library_object_offset = -1;
    strcpy(previous_library_object, "nothing");

    /* Read the first candidate symbol line. */
    if (!fgets(line_buffer, 0x4000, map_file))
        goto normal_eof;

    for (;;)
    {
        number_end = NULL;
        segment_token = strtok(line_buffer, ":");
        if (!segment_token || *segment_token != ' ')
        {
            /* Not a symbol line — read the next line and re-classify. */
            if (!fgets(line_buffer, 0x4000, map_file))
                goto normal_eof;
            continue;
        }

        offset_token = strtok(NULL, " \t\n\r");
        if (!offset_token)
            goto cleanup;
        symbol_address = strtoul(offset_token, &number_end, 16);

        name_token = strtok(NULL, " \t\n\r");
        if (!name_token)
        {
            /* No name field — this is the "entry point at" marker that precedes the
             * "Static symbols" continuation section. Skip through to its first entry. */
            if (!strstr(line_buffer, "entry point at"))
                goto cleanup;
            fgets(line_buffer, 0x4000, map_file);
            if (!isspace((unsigned char)line_buffer[0])) /* _pctype[c] & _SPACE */
                goto cleanup;
            fgets(line_buffer, 0x4000, map_file);
            if (!strstr(line_buffer, "Static symbols"))
                goto cleanup;
            fgets(line_buffer, 0x4000, map_file);
            if (!isspace((unsigned char)line_buffer[0]))
                goto cleanup;
            fgets(line_buffer, 0x4000, map_file);
            segment_token = strtok(line_buffer, ":");
            if (!segment_token)
                goto cleanup;
            if (*segment_token != ' ')
                goto cleanup;
            offset_token = strtok(NULL, " \t\n\r");
            if (!offset_token)
                goto cleanup;
            symbol_address = strtoul(offset_token, &number_end, 16);
            name_token = strtok(NULL, " \t\n\r");
        }

        if (name_token)
        {
            strncpy(symbol_name, name_token, 0xFF);
            symbol_name[255] = 0;
        }

        rva_token = strtok(NULL, " \t\n\r");
        if (!rva_token)
            goto cleanup;
        rva_value = strtoul(rva_token, &number_end, 16);
        rva_base = rva_value;

        /* When we encounter our own symbol, record the map->image load-address fixup. */
        if (strcmp(symbol_name, "_load_symbol_table") == 0)
            stack_walk_globals.fixup = rva_value - (unsigned int)(uintptr_t)load_symbol_table;

        if (!number_end)
            goto cleanup;
        number_end += 5; /* skip the fixed-width symbol flags field to reach lib:object */
        library_object_token = strtok(number_end, " \t\n\r");
        if (!library_object_token)
            goto cleanup;
        strncpy(library_object, library_object_token, 0xFF);
        library_object[255] = 0;

        /* Grow the symbol array when full (in 4096-record increments). */
        if ((unsigned int)symbol_table->number_of_symbols >= symbols_capacity)
        {
            symbols_capacity += 4096;
            new_symbols = dlRealloc(
                symbol_table->symbols,
                sizeof(debug_symbol) * symbols_capacity,
                "D:\\Projects\\code\\HCEX\\sources\\cseries\\stack_walk_windows.c",
                0x1E3);
            if (!new_symbols)
            {
                allocation_failed = 1;
                goto cleanup;
            }
            symbol_table->symbols = new_symbols;
        }

        name_length = (unsigned int)strlen(symbol_name);
        library_length = (unsigned int)strlen(library_object);

        /* Grow the string pool when the name + library strings would not fit. */
        if (library_length + name_length + string_storage_used + 2 >= string_storage_capacity)
        {
            string_storage_capacity += 0x4000;
            new_string_storage = dlRealloc(
                symbol_table->string_storage,
                string_storage_capacity,
                "D:\\Projects\\code\\HCEX\\sources\\cseries\\stack_walk_windows.c",
                0x1F4);
            if (!new_string_storage)
            {
                allocation_failed = 1;
                goto cleanup;
            }
            symbol_table->string_storage = new_string_storage;
        }

        new_symbol = &symbol_table->symbols[symbol_table->number_of_symbols++];
        new_symbol->address = symbol_address;
        new_symbol->rva_base = rva_base;

        strcpy(&symbol_table->string_storage[string_storage_used], symbol_name);
        new_symbol->name_string_offset = string_storage_used;
        string_storage_used += name_length + 1;

        if (strcmp(previous_library_object, library_object) != 0)
        {
            strcpy(&symbol_table->string_storage[string_storage_used], library_object);
            new_symbol->library_object_string_offset = string_storage_used;
            previous_library_object_offset = (int)string_storage_used;
            string_storage_used += library_length + 1;
            strcpy(previous_library_object, library_object);
        }
        else
        {
            /* Consecutive symbols in the same library share one interned string. */
            new_symbol->library_object_string_offset = previous_library_object_offset;
        }

        /* Advance to the next line. */
        if (!fgets(line_buffer, 0x4000, map_file))
            goto normal_eof;
    }

normal_eof:
    parse_incomplete = 0;
cleanup:
    if (allocation_failed || parse_incomplete)
        free_symbol_table(symbol_table);
close_file:
    fclose(map_file);
finish:
    if (symbol_table->number_of_symbols <= 0)
        return 0;

    qsort(symbol_table->symbols, symbol_table->number_of_symbols,
          sizeof(debug_symbol),
          (int (*)(const void *, const void *))symbol_sort_proc);

    /* Drop trailing zero-rva sentinel entries pushed to the end by the sort. */
    if (symbol_table->symbols[symbol_table->number_of_symbols - 1].rva_base == 0)
    {
        do
        {
            trimmed_count = symbol_table->number_of_symbols - 1;
            symbol_table->number_of_symbols = trimmed_count;
        }
        while (symbol_table->symbols[trimmed_count - 1].rva_base == 0);
    }

    return symbol_table->number_of_symbols > 0;
}
