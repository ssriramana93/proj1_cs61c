#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

void write_inst_string(FILE* output, const char* name, char** args, int num_args) {
    fprintf(output, "%s", name);
    for (int i = 0; i < num_args; i++) {
        fprintf(output, " %s", args[i]);
    }
    fprintf(output, "\n");
}

void write_inst_hex(FILE *output, uint32_t instruction) {
    fprintf(output, "%08x\n", instruction);
}

int is_valid_label(const char* str) {
    if (!str) {
        return 0;
    }

    int first = 1;
    while (*str) {
        if (first) {
            if (!isalpha((int) *str) && *str != '_') {
                return 0;   // does not start with letter or underscore
            } else {
                first = 0;
            }
        } else if (!isalnum((int) *str) && *str != '_') {
            return 0;       // subsequent characters not alphanumeric
        }
        str++;
    }
    return first ? 0 : 1;   // empty string is invalid
}

/* Translate the input string into a signed number. The number is then 
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND. 

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take 
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants. 

   You should store the result into the location that OUTPUT points to. The 
   function returns 0 if the conversion proceeded without errors, or -1 if an 
   error occurred.
 */
 
       /*The string may begin with an arbitrary amount of white space (as
       determined by isspace(3)) followed by a single optional '+' or
       '-' sign.  If base is zero or 16, the string may then include a
       "0x" or "0X" prefix, and the number will be read in base 16;
       otherwise, a zero base is taken as 10 (decimal) unless the next
       character is '0', in which case it is taken as 8 (octal).

       The remainder of the string is converted to a long value in the
       obvious manner, stopping at the first character which is not a
       valid digit in the given base.  (In bases above 10, the letter
       'A' in either uppercase or lowercase represents 10, 'B'
       represents 11, and so forth, with 'Z' representing 35.)

       If endptr is not NULL, strtol() stores the address of the first
       invalid character in *endptr.  If there were no digits at all,
       strtol() stores the original value of nptr in *endptr (and
       returns 0).  In particular, if *nptr is not '\0' but **endptr is
       '\0' on return, the entire string is valid.*/
int translate_num(long int* output, const char* str, long int lower_bound, 
    long int upper_bound) {
    if (!str || !output) {
        return -1;
    }
    /* YOUR CODE HERE */
    auto clen = strlen(str);
    const char* endp;
    long int res = strol(str, endp, 0);
    if (lower_bound > res || res > upper_bound) {
      return -1;
    }
    if (res == 0) {
        if (strcmp(str, endp) == 0) {
            return -1;
        }
    }

    
  
  
  *output = res;
   return 0;
  
}

/* Translates the register name to the corresponding register number. Please
   see the MIPS Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char* str) {
    if (strcmp(str, "$zero") == 0)      return 0;
    else if (strcmp(str, "$0") == 0)    return 0;
    else if (strcmp(str, "$at") == 0)   return 1;
    else if (strcmp(str, "$v0") == 0)   return 2;
    else if (strcmp(str, "$a0") == 0)   return 4;
    else if (strcmp(str, "$v1") == 0) return 3;
    else if (strcmp(str, "$a1") == 0) return 5;
    else if (strcmp(str, "$a2") == 0) return 6;
    else if (strcmp(str, "$a3") == 0) return 7;
    else if (strcmp(str, "$t0") == 0) return 8;
    else if (strcmp(str, "$t1") == 0) return 9;
    else if (strcmp(str, "$t2") == 0) return 10;
    else if (strcmp(str, "$t3") == 0) return 11;
    else if (strcmp(str, "$t4") == 0) return 12;
    else if (strcmp(str, "$t5") == 0) return 13;
    else if (strcmp(str, "$t6") == 0) return 14;
    else if (strcmp(str, "$t7") == 0) return 15;
    else if (strcmp(str, "$s0") == 0) return 16;
    else if (strcmp(str, "$s1") == 0) return 17;
    else if (strcmp(str, "$s2") == 0) return 18;
    else if (strcmp(str, "$s3") == 0) return 19;
    else if (strcmp(str, "$s4") == 0) return 20;
    else if (strcmp(str, "$s5") == 0) return 21;
    else if (strcmp(str, "$s6") == 0) return 22;
    else if (strcmp(str, "$s7") == 0) return 23;
    else if (strcmp(str, "$t8") == 0) return 24;
    else if (strcmp(str, "$t9") == 0) return 25;
    else if (strcmp(str, "$sp") == 0) return 26;
    else if (strcmp(str, "$gp") == 0) return 27;
    else if (strcmp(str, "$fp") == 0) return 28;
    else if (strcmp(str, "$ra") == 0) return 29;
    else return -1;
}
