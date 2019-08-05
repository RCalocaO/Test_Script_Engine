// Stacks for ICode Generation
// (C) 1999 by A'r-Ci-O'oh

#ifndef __STACKS_H__
#define __STACKS_H__

#include "../Scanner/script.h"

struct TokenItem
{
  TokenType type;
  char name[MAX_TOKEN_LENGTH+1];
  int data;
  TokenItem* next;
  TokenItem(const char *n,TokenType t,int d)
    { strcpy(name,n); type=t; data=d; next=NULL; };
};

class TokenStack
{
   public:
     TokenStack() { top=NULL; }
     ~TokenStack() { while (top!=NULL) Pop(); };
     TokenItem* Push(const char *name,TokenType type,int data)
       {
          TokenItem *z=new TokenItem(name,type,data);
          z->next=top;
          top=z;
          return z;
       }
     TokenItem Pop()
       {
          if (top==NULL)
            return TokenItem("",T_ERROR,-1);
          TokenItem z=*top;
          TokenItem *old=top;
          top=top->next;
          delete old;
          return z;
       }
     TokenItem* Top() { return top; };
   protected:
     TokenItem* top;
};

extern TokenStack *OperandStack;
extern TokenStack *OperStack;
extern TokenStack *JumpStack;

/*
// Stack for Handling Local Variable Tables
struct TableItem
{
  void *data;
  TableItem* next;
  TableItem(void *d)
    { data=d; next=NULL; };
};

class TableStack
{
   public:
     TableStack() { top=NULL; }
     ~TableStack() { while (top!=NULL) Pop(); };
     TableItem* Push(void *d)
       {
          TableItem *z=new TableItem(d);
          z->next=top;
          top=z;
          return z;
       }
     TableItem Pop()
       {
          if (top==NULL)
            return TableItem(NULL);
          TableItem z=*top;
          TableItem *old=top;
          top=top->next;
          delete old;
          return z;
       }
     TableItem* Top() { return top; };
   protected:
     TableItem* top;
};

extern TableStack *LocalTable;
*/
#endif