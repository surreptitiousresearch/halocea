/* base_address_from_symbol_name @0x83810E38 — linear search of a debug symbol table for the
 * symbol whose name matches `name`, returning its rva_base (or 0xFFFFFFFF if not found). The
 * scan starts at index 1 (index 0 is the reserved/header symbol). */

#include "headers/debug_symbol_table.h"

unsigned int base_address_from_symbol_name(const char *name, debug_symbol_table *symbol_table)
{
    unsigned int result = 0xFFFFFFFF;

    if ( symbol_table->number_of_symbols > 1 )
    {
        debug_symbol *symbols = symbol_table->symbols;
        for ( int i = 1; i < symbol_table->number_of_symbols; ++i )
        {
            const char *lhs = name;
            const char *rhs = &symbol_table->string_storage[symbols[i].name_string_offset];
            int diff;
            do
            {
                int c = (unsigned char)*lhs;
                diff = c - (unsigned char)*rhs;
                if ( c == 0 )
                    break;
                ++lhs;
                ++rhs;
            }
            while ( !diff );

            if ( !diff )
                result = symbols[i].rva_base;
        }
    }

    return result;
}
