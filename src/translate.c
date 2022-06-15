#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li and blt pseudoinstructions. Your pseudoinstruction 
   expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are 
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number 
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addiu instruction. Otherwise, expand it into 
        a lui-ori pair.

   And for blt:
    - your expansion should use the fewest number of instructions possible.

   MARS has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if MARS behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that 
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
unsigned write_pass_one(FILE* output, const char* name, char** args, int num_args) {
    if (strcmp(name, "li") == 0) {
        /* YOUR CODE HERE */
        return 0;
    } else if (strcmp(name, "blt") == 0) {
        /* YOUR CODE HERE */
        return 0;
    } else {
        write_inst_string(output, name, args, num_args);
        return 1;
    }
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.
   
   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS. 

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step. If a symbol should be relocated, it should be added to the
   relocation table (RELTBL), and the fields for that symbol should be set to
   all zeros. 

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write 
   anything to OUTPUT but simply return -1. MARS may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error. 
 */
int translate_inst(FILE* output, const char* name, char** args, size_t num_args, uint32_t addr,
    SymbolTable* symtbl, SymbolTable* reltbl) {
    if (strcmp(name, "addu") == 0)       return write_rtype (0x21, output, args, num_args);
    else if (strcmp(name, "or") == 0)    return write_rtype (0x25, output, args, num_args);
    else if (strcmp(name, "slt") == 0)   return write_rtype (0x2a, output, args, num_args);
    else if (strcmp(name, "sltu") == 0)  return write_rtype (0x2b, output, args, num_args);
    else if (strcmp(name, "sll") == 0)   return write_shift (0x00, output, args, num_args);
    else if (strcmp(name, "addiu") == 0) return write_itype(0x09, output, args, num_args);
    else if (strcmp(name, "ori") == 0) return write_itype(0x0D, output, args, num_args);
    else if (strcmp(name, "lui") == 0) return write_itype_big(0x0F, output, args, num_args);
    else if (strcmp(name, "lb") == 0) return write_itype_mem();
    else if (strcmp(name, "lbu") == 0) return write_itype_mem();
    else if (strcmp(name, "lw") == 0) return write_itype_mem();
    else if (strcmp(name, "sb") == 0) return write_itype_mem();
    else if (strcmp(name, "sw") == 0) return write_itype_mem();
    else if (strcmp(name, "beq") == 0) return write_itype_branch();
    else if (strcmp(name, "bne") == 0) return write_itype_branch();

    else if (strcmp(name, "jr") == 0) return 
    /* YOUR CODE HERE */
    else                                 return -1;
}

int write_itype(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    // opcode rs rt imm
    if (num_args != 3) {
        return -1;
    }
    long int imm;
    int rt = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    long int two_pow_16 = pow(2, 16);
    int err = translate_num(args[2], imm, -two_pow_16, two_pow_16);
    if (err == -1) {
        return -1;
    }

    uint32_t instruction = 0;
    int bit = 0;
    for (bit = 0; bit < 16; ++bit) {
       instruction += ((imm >> 1) & 1) * pow(2, bit);
    }
    for (bit = 16; bit < 21; ++bit) {
       instruction += ((rt >> 1) & 1) * pow(2,bit);
    }
    for (bit = 21; bit < 26; ++bit) {
       instruction += ((rs >> 1) & 1) * pow(2,bit);
    }
    for (bit = 26; bit < 32; ++bit) {
       instruction += ((opcode >> 1) & 1) * pow(2,bit);
    }
    

    write_inst_hex(output, instruction);
    return 0;
}

int write_itype_mem(uint8_t opcode, FILE* output, char** args, size_t num_args) {

}

int write_itype_big(uint8_t opcode, FILE* output, char** args, size_t num_args) {
    
}
/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to 
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t funct, FILE* output, char** args, size_t num_args) {
    // opcode rs rt rd shamt funct
    // Perhaps perform some error checking?
    if (num_args != 3) {
      return -1;
    }


    int rd = translate_reg(args[0]);
    int rs = translate_reg(args[1]);
    int rt = translate_reg(args[2]);
    if (rd < 0 || rd > 31 || rs < 0 || rs > 31 || rt < 0 || rt > 31) {
      return -1;
    }


    uint32_t instruction = 0;
    int bit = 0;
    for (bit = 0; bit < 6; ++bit) {
       instruction += ((funct >> 1) & 1) * pow(2, bit);
    }
    for (bit = 11; bit < 16; ++bit) {
       instruction += ((rd >> 1) & 1) * pow(2,bit);
    }
    for (bit = 16; bit < 21; ++bit) {
       instruction += ((rt >> 1) & 1) * pow(2,bit);
    }
    for (bit = 21; bit < 26; ++bit) {
       instruction += ((rs >> 1) & 1) * pow(2,bit);
    }
    

    write_inst_hex(output, instruction);
    return 0;
}

/* A helper function for writing shift instructions. You should use 
   translate_num() to parse numerical arguments. translate_num() is defined
   in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_shift(uint8_t funct, FILE* output, char** args, size_t num_args) {
  	// Perhaps perform some error checking?
   // opcode rs rt rd shamt funct

    if (num_args != 3) {
      return -1;
    }
    long int shamt;
    int rd = translate_reg(args[0]);
    int rt = translate_reg(args[1]);
    int err = translate_num(&shamt, args[2], 0, 31);
    if (err == -1) {
      return -1;
    }
    if (rd < 0 || rd > 31 || rt < 0 || rt > 31) {
      return -1;
    }
    uint32_t instruction = 0;
    int bit = 0;
    for (bit = 6; bit < 11; ++bit) {
       instruction += ((shamt >> 1) & 1) * pow(2, bit);
    }
    for (bit = 11; bit < 16; ++bit) {
       instruction += ((rd >> 1) & 1) * pow(2,bit);
    }
    for (bit = 16; bit < 21; ++bit) {
       instruction += ((rt >> 1) & 1) * pow(2,bit);
    }
   
    write_inst_hex(output, instruction);
    return 0;
}
