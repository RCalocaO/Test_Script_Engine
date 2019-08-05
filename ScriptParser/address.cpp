// Address
// (C) 1999 by A'r-Ci-O'oh

#include "address.h"

bool LocalZone;

static int Const_Int=0;
static int Const_Real=MEM_SIZE;
static int Global_Int=2*MEM_SIZE;
static int Global_Real=3*MEM_SIZE;
static int Local_Int=4*MEM_SIZE;
static int Local_Real=5*MEM_SIZE;
static int Temp_Int=6*MEM_SIZE;
static int Temp_Real=7*MEM_SIZE;

void ResetZone()
{
   Local_Int=4*MEM_SIZE;
   Local_Real=5*MEM_SIZE;
}

int GetAddress(TokenType type)
{
  switch(type)
    {
       case T_NUM_INT: return Const_Int++;
       case T_NUM_REAL: return Const_Real++;
       case T_INT:  if (LocalZone)
                      return Local_Int++;
                     else
                    return Global_Int++;
       case T_REAL:  if (LocalZone)
                       return Local_Real++;
                      else
                       return Global_Real++;
       case T_ENTITY_VECTOR:
       case T_VECTOR: {
                        if (LocalZone)
                          return (Local_Real+=3)-3;
                         else
                          return (Global_Real+=3)-3;
                      }
    }
  return -1;
}

// -----------------------------------------------------------------

// Temporal Name Handler
TokenItem GetNextTemporal(TokenType type)
{
  int addr=-1;
  switch(type)
    {
       case T_NUM_INT:
       case T_INT:
                      addr=Temp_Int++;
                      break;
       case T_NUM_REAL:
       case T_REAL:
                      addr=Temp_Real++;
                      break;
       case T_VECTOR:
       case T_ENTITY_VECTOR:
                      addr=(Temp_Real+=3)-3;
                      break;
    }
  char name[30]="TEMP__";
  char num[15]; _itoa(addr,num,10);
  strcat(name,num);
  if ((type==T_VECTOR) || (type==T_ENTITY_VECTOR))
    strcat(name,"_V");
  return TokenItem(name,type,addr);
}

TokenType GetType(int address)
{
  switch(address/MEM_SIZE)
    {
      case 0:
      case 2:
      case 4:
      case 6:
              return T_INT;
      case 1:
      case 3:
      case 5:
      case 7:
              return T_REAL;
    }
  return T_ERROR;
}

