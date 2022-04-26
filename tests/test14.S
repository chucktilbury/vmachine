/*
 *  See if we can print a string.
 */

string string_var = "this is a string"

entry
    print string_var
    push string_var
    prints
    pop
    exit