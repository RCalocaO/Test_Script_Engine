// Semantic Verifications
// (C) 1999 by A'r-Ci-O'oh

#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "../Scanner/script.h"
#include "stacks.h"

extern TokenType SEMANTIC_VALIDATE_OP1_OP2_OPER(TokenItem& a,
                 TokenItem& b,TokenItem& oper);

extern void SEMANTIC_VALIDATE_OP1_RESULT(TokenItem& op1,
       TokenItem& result,TokenItem& oper);

#endif

