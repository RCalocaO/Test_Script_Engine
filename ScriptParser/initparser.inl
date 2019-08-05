// Assignments only to Global int, real or vectors

void Parse_VarAssign()
{
                               // Breakpoint
                               START=true;
        // Get Info from Global Variables
       LV_Entry *v=SymTable->Search("GLOBAL")->local->Search(tokenstring);
       if (v==NULL)
        {
          SemanticError("Identifier not Declared!");
          return;
        }
   Match(T_ID);
   if (token==T_ASSIGN)
    if ((v->type==T_VECTOR) || (v->type==T_ENTITY_VECTOR))
        SemanticError("Vector cannot be initialized as simple type!");
     else
     {
             // Check that this is not a vector!
             // Now push into Stack
             OperStack->Push("=",T_ASSIGN,T_ASSIGN);
             OperandStack->Push(v->name,v->type,v->address); // Must Get Real Address!
        Match(T_ASSIGN);
        Expression();
                             // Generate simple oper, operand & rsult
                             GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();
        Match(T_SEMICOLON);
     }
    else if (token==T_LP)
     if ((v->type!=T_VECTOR) && (v->type!=T_ENTITY_VECTOR))
         SemanticError("Simple Variable cannot be initialized as Vector!");
       else
      {
         Match(T_LP);
             // Now push into Stack
             OperStack->Push("=",T_ASSIGN,T_ASSIGN);
             OperandStack->Push(v->name,v->type,v->address);
         Expression();
                             // Generate simple oper, operand & rsult
                             GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();
         Match(T_COMMA);
             // Now push into Stack
             OperStack->Push("=",T_ASSIGN,T_ASSIGN);
             OperandStack->Push(v->name,v->type,v->address+1);
         Expression();
                             // Generate simple oper, operand & rsult
                             GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();
         Match(T_COMMA);
             // Now push into Stack
             OperStack->Push("=",T_ASSIGN,T_ASSIGN);
             OperandStack->Push(v->name,v->type,v->address+2);
         Expression();
                             // Generate simple oper, operand & rsult
                             GEN_ICODE_OPERAND_OP1_NONE_RESULTINSTACK();
         Match(T_RP);
         Match(T_SEMICOLON);
      }
}
void Parse_VarInit()
{
   if (token!=T_RB)
     {
       Parse_VarAssign();
       Parse_VarInit();
     }
}


