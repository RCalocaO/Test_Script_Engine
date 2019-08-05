
// For Variable declarations and access
void Variable();
void Expression();
void AlgebraicExpression();
void Term();
void Factor();


// This function is used when a VECTOR var is in the stack, and a member
// is being referenced in the next token
void ADD_VECTOR_MEMBER(char *member)
{
  // First retrieve base address
  TokenItem base=OperandStack->Pop();
  if ((base.type!=T_VECTOR) && (base.type!=T_ENTITY_VECTOR))
    {
          SyntaxError("Not a VECTOR Variable!: ");
          PrintToken(token,tokenstring);
          return;
    }
  int displ=-1;
  if (strcmp(member,"x")==0)
    displ=0;
   else if (strcmp(member,"y")==0)
     displ=1;
    else if (strcmp(member,"z")==0)
      displ=2;
     else
       {
          SyntaxError("Illegal Member in VECTOR: ");
          PrintToken(token,tokenstring);
          return;
       }
  // Now add this displacement to the start address
  base.data+=displ;
  strcat(base.name,".");
  strcat(base.name,member);
  OperandStack->Push(base.name,T_REAL,base.data);
}


// A Variable can be of type z.x.y.w.v
void Variable()
{
        // Find if it exists. v has Local Info Structure, NULL if not found
        LV_Entry *v=SymTable->GetEntry(tokenstring);
        if (v!=NULL)
            // Push to Operand Stack (I'm only interested in address!)
            OperandStack->Push(v->name,v->type,v->address);
          else
            OperandStack->Push("ERROR",T_ERROR,-1);
        TokenType basetype=v->type;
   Match(T_ID);
   if (token==T_DOT)
     {
       Match(T_DOT);
     switch(basetype)
      {

        case T_ENTITY_VECTOR:
        case T_VECTOR:
                            // OK, add to its base address the next member
                            ADD_VECTOR_MEMBER(tokenstring);
                       Match(T_ID);
                       break;
        default:  SyntaxError("Identifier has no member: ");
                  PrintToken(token,tokenstring);
                  token=GetToken();
                  break;
      }
     }
}

// 1 & 0
int ConstInt=1;
int ConstReal=1;

// A Constant is typed Real, Int
void Constant()
{
      // Get Constant Type
      type=token;
      // Search if constant is inserted into const identifiers sym table
      LocalVariableTable *where=SymTable->Search("CONSTANT")->local;
      LV_Entry *constnt=where->Search(tokenstring,false);
      // If it is not found, insert it
      if (constnt==NULL)
        {
          where->Insert(tokenstring,type);
          constnt=where->Search(tokenstring,false);
          if ((type==T_INT) || (type==T_NUM_INT))
             if (ConstInt<constnt->address)
               ConstInt=constnt->address;
           else if (ConstReal<constnt->address)
                ConstReal=constnt->address;
        }
      // Push to Operand Stack (I'm only interested in address!)
      OperandStack->Push(constnt->name,constnt->type,constnt->address);
   Match(token);
}


// A Factor is: A variable, a constant, an expression
void Factor()
{
  // Unary Minus
  int negated=0;
  if (token==T_MINUS)
   {
      negated=1;
      Match(T_MINUS);
   }
  switch(token)
    {
       case T_NUM_REAL:
       case T_NUM_INT:
                       Constant();
                       break;
       case T_ID: Variable();
                  break;
       case T_LP: Match(T_LP);
                        // Push False Stack Bottom
                        OperStack->Push("(",T_LP,T_LP);
                  Expression();
                  Match(T_RP);
                        // Pop False Stack Bottom
                        OperStack->Pop();
                  break;
    }
  if (negated==0) return;
       // Now Generate ICode for Negating result in stack
       GEN_ICODE_NEGATE();
}

// A Term is the multiplication of factors
void Term()
{
  Factor();
       // Now Generate ICode for Pending Multiplicative Terms
       GEN_ICODE_MULT();
  switch(token)
     {
        case T_TIMES:
        case T_OVER:
        //case T_OR:
        case T_MOD:
                        // Push Operation in Operation Stack.
                        OperStack->Push(tokenstring,token,token);
                    Match(token);
                    Term();
                    break;
     }
}

// An Algebraic Expression is a sum of terms
void AlgebraicExpression()
{
   Term();
        // Now Generate ICode for Pending Additive Terms
        GEN_ICODE_ADD();
   switch(token)
     {
        case T_PLUS:
        case T_MINUS:
        //case T_AND:
        //case T_OR:
                        // Push Operation in Operation Stack. Address = -1
                        OperStack->Push(tokenstring,token,token);
                    Match(token);
                    AlgebraicExpression();
                    break;
     }
}

void Expression()
{
   AlgebraicExpression();
   switch(token)
     {
        case T_EQ:
        case T_NOTEQ:
        case T_GTEQ:
        case T_GT:
        case T_LWEQ:
        case T_AND:
        case T_OR:
        case T_LW:
                        // Push Operation in Operation Stack.
                        OperStack->Push(tokenstring,token,token);
                   Match(token);
                   AlgebraicExpression();
                        // Generate simple oper, 2 operands & result code
                        GEN_ICODE_OPERAND_OP1_OP2_GETRESULT();
                   break;
     }
}





