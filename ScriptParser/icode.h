// ICode Procs
// (C) 1999 by A'r-Ci-O'oh

#ifndef __ICODE_H__
#define __ICODE_H__

extern void GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();

extern void GEN_ICODE_OPERAND_OP1_OP2_GETRESULT();

extern void GEN_ICODE_MULT();
extern void GEN_ICODE_ADD();

extern void GEN_IF_TEST_CODE();
extern void GEN_IF_EXIT_CODE();
extern void GEN_ELSE_CODE();

extern void GEN_START_WHILE_CODE();
extern void GEN_WHILE_TEST_CODE();
extern void GEN_WHILE_END_CODE();

extern void GEN_START_REPEAT_CODE();
extern void GEN_REPEAT_END_CODE();

extern void GEN_UNARY(TokenType oper);

extern void GEN_ICODE_LOOP_MAIN(int EntryPoint);

extern void GEN_PRINT_CODE();

extern void GEN_ICODE_NEGATE();

#endif
