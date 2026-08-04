/* symbol_name_from_address @0x83810D20 — resolve a raw (relocation-fixed-up) address to a
 * "library!object + offset" string using a sorted-by-rva_base debug_symbol_table: binary-search-free
 * linear scan for the symbol whose [rva_base, next symbol's rva_base) range contains the address. Returns
 * the shared static `symbol_buffer`, left as "<unknown>" if the table is empty, the address falls outside
 * the table's range, or no exact bracketing symbol is found. */

#include "headers/debug_symbol_table.h"
#include "headers/stack_walk_globals.h"
#include "headers/blam_data_globals.h"


extern char *strcpy(char *destination, const char *source);
extern int _snprintf_0(char *string, unsigned int count, const char *format, ...);

char * symbol_name_from_address(unsigned int fake_address, debug_symbol_table *symbol_table)
{
    strcpy(symbol_buffer, "<unknown>");

    unsigned int address = stack_walk_globals.fixup + fake_address;
    int number_of_symbols = symbol_table->number_of_symbols;

    if ( number_of_symbols > 0 )
    {
        debug_symbol *symbols = symbol_table->symbols;

        if ( address >= symbols[0].rva_base && address < symbols[number_of_symbols - 1].rva_base + 0xFFFF )
        {
            int symbol_index = 1;
            if ( number_of_symbols > 1 )
            {
                int i;
                for ( i = 1; symbols[i - 1].rva_base > address || address >= symbols[i].rva_base; i++ )
                {
                    if ( ++symbol_index >= number_of_symbols )
                        return symbol_buffer;
                }

                _snprintf_0(symbol_buffer, 0x3FFF, "%s + %04lX : %s",
                    &symbol_table->string_storage[symbol_table->symbols[symbol_index - 1].name_string_offset],
                    address - symbol_table->symbols[symbol_index - 1].rva_base,
                    &symbol_table->string_storage[symbol_table->symbols[symbol_index - 1].library_object_string_offset]);
            }
        }
    }

    return symbol_buffer;
}
