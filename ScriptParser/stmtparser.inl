
void Parse_Stmt();
void Parse_Stmts();

void Parse_AssignUnaryCall()
{
  Variable();
  switch(token)
    {
       case T_PLUSPLUS:
       case T_MINUSMINUS:
                               // Breakpoint
                               START=true;
                               // Generate Code for unary inc/dec
                               GEN_UNARY(token);
                          Match(token);
                          Match(T_SEMICOLON);
                          break;
       case T_ASSIGN:
                      Match(T_ASSIGN);
                             // Push Operation in Stack.
                             OperStack->Push("=",T_ASSIGN,T_ASSIGN);
                               // Breakpoint
                               START=true;
                      Expression();
                             // Generate simple oper, operand & rsult
                             GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();
                      Match(T_SEMICOLON);
                      break;
    }
}

void ParseWhile()
{
                               // Breakpoint
                               START=true;
  Match(T_WHILE);
  Match(T_LP);
        // Start of While Condition
        GEN_START_WHILE_CODE();
  Expression();
        // Now Operand Stack has the result of condition
        GEN_WHILE_TEST_CODE();
  Match(T_RP);
                               // Breakpoint
                               START=true;
  Parse_Stmt();
        // Return to Condition
        GEN_WHILE_END_CODE();
}

void ParseIf()
{
                               // Breakpoint
                               START=true;
   Match(T_IF);
   Match(T_LP);
   Expression();
        // Now Operand Stack has the result of condition
        GEN_IF_TEST_CODE();
   Match(T_RP);
                               // Breakpoint
                               START=true;
   Parse_Stmt();
   if (token==T_ELSE)
     {
        Match(T_ELSE);
                               // Breakpoint
                               START=true;
             // Code to exit else
             GEN_ELSE_CODE();
        Parse_Stmt();
     }
        // End of if
        GEN_IF_EXIT_CODE();
}

void ParseRepeat()
{
                               // Breakpoint
                               START=true;
  Match(T_REPEAT);
       // Save current PC
       GEN_START_REPEAT_CODE();
                               // Breakpoint
                               START=true;
  Parse_Stmt();
  Match(T_UNTIL);
  Match(T_LP);
  Expression();
  Match(T_RP);
                               // Breakpoint
                               START=true;
       // Test and goto start if false
       GEN_REPEAT_END_CODE();
  Match(T_SEMICOLON);
}

void Parse_Print()
{
                               // Breakpoint
                               START=true;
  Match(T_PRINT);
  Match(T_LP);
  Expression();
       // Print result of Expression
       GEN_PRINT_CODE();
  Match(T_RP);
  Match(T_SEMICOLON);
}

void Parse_Stmt()
{
  switch(token)
    {
       case T_ID: // Assignment, Unary or Module Call
                  Parse_AssignUnaryCall();
                  break;
       case T_WHILE: ParseWhile();
                     break;
       case T_IF: ParseIf();
                  break;
       case T_REPEAT: ParseRepeat();
                      break;
       case T_LB: Match(T_LB);
                  Parse_Stmts();
                  Match(T_RB);
                  break;
       case T_PRINT: Parse_Print();
                     break;
    };
}

void Parse_Stmts()
{
   if (token!=T_RB)
     {
        Parse_Stmt();
        Parse_Stmts();
     }
}


void Parse_StmtBlock()
{
   Match(T_LB);
   Parse_Stmts();
   Match(T_RB);
}



