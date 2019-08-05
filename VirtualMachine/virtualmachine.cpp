// Virtual Machine Class for 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail->com)

#include "virtualmachine.h"
#include "../Scanner/script.h"
#include "../trohs3d.h"
#include "../entity.h"
#include "../world.h"
#include "../ScriptParser/address.h"
#include "../engine/sector.h"
#include "../engine/polygon3d.h"
#include "../engine/view.h"
#include "../engine/camera.h"
#include "../ScriptParser/quad.h"
#include "../ScriptParser/proc_dir.h"
#include <stdio.h>

VirtualMachine Trohs3dVM;

VirtualMachine::VirtualMachine()
{
  PC=-1;
  Temporal=Global=Constant=Local=NULL;
}

VirtualMachine::~VirtualMachine()
{
  // Clear Memory
/*  if (Global)
    {
      if (Global->ints)
        delete [] Global->ints;
      if (Global->reals)
        delete [] Global->reals;*/
      delete Global;
/*    }
  if (Constant)
    {
      if (Constant->ints)
        delete [] Constant->ints;
      if (Constant->reals)
        delete [] Constant->reals;*/
      delete Constant;
/*    }
  if (Temporal)
    {
      if (Temporal->ints)
        delete [] Temporal->ints;
      if (Temporal->reals)
        delete [] Temporal->reals;*/
      delete Temporal;
/*    }
  if (Local)
    {
      if (Local->ints)
        delete [] Local->ints;
      if (Local->reals)
        delete [] Local->reals;*/
      delete Local;
//    }
  Global=Temporal=Constant=Local=NULL;
  Reset();
}

void VirtualMachine::Reset()
{
  // Reset Program Counter
  if (ICode)
    PC=0;
   else
    PC=-1;
}

bool VirtualMachine::Load()
{
  // ICode is already loaded with quadruples
  if (ICode)
    PC=0;
   else
    PC=-1;
  // Reserve Memory for Constants and Globals
  Constant=new MemoryTable();
  Global=new MemoryTable();
  Temporal=new MemoryTable();
  // Local=new MemoryTable();

  int size=SymTable->Search("CONSTANT")->MaxInt;
  if (size>0)
    Constant->ints=new int[size];
  size=SymTable->Search("CONSTANT")->MaxReal;
  if (size>0)
    Constant->reals=new double[size];

  size=SymTable->Search("GLOBAL")->MaxInt-2*MEM_SIZE;
  if (size>0)
    Global->ints=new int[size];
  size=SymTable->Search("GLOBAL")->MaxReal-3*MEM_SIZE;
  if (size>0)
    Global->reals=new double[size];

  size=SymTable->Search("TEMPORAL")->MaxInt-6*MEM_SIZE;
  if (size>0)
    Temporal->ints=new int[size];
  size=SymTable->Search("TEMPORAL")->MaxReal-7*MEM_SIZE;
  if (size>0)
    Temporal->reals=new double[size];

  // Missing local!!!!

  // Load Constants Info int Memory!
  LocalVariableTable *table=SymTable->Search("CONSTANT")->local;
  for (int addr=0; addr<SymTable->Search("CONSTANT")->MaxInt; addr++)
    {
       LV_Entry *value=table->Search(addr,true);
       int zx=atoi(value->name);
       Constant->ints[addr]=zx;
    }
  for (int addr=MEM_SIZE; addr<SymTable->Search("CONSTANT")->MaxReal+MEM_SIZE; addr++)
    {
       LV_Entry *value=table->Search(addr,true);
       double zx=atof(value->name);
       Constant->reals[addr-MEM_SIZE]=zx;
    }
  // Load the Entity Vector Information!
  LoadEntityInfo();

  return 0;
}

// One quadruple at time
void VirtualMachine::ICodeStep()
{
  // If no program in memory, halt
  if (PC<0)
    return;
  // Retrieve First Quadruple
  Quad q=ICode->data[PC];
  Execute(q);
  PC++;
}

void VirtualMachine::Execute(Quad &q)
{
  switch(q.type[0])
    {
       case Q_NONE:
       case Q_EMPTY: // Should never happen!
                     break;
       case Q_OPER:  ExecuteOperation(q);
                     break;
    }
  // Update the Entity Vector Information!
  SaveEntityInfo();
}

void VirtualMachine::ExecuteOperation(Quad &q)
{
  void *op1,*op2,*dest;
  switch(q.data[0])
   {
     case T_JMP: PC=q.data[3]-1;
                 break;
     case T_PRINT:{
                     op1=GetPointer(q.data[1]);
                     double value;
                     char l[100];
                     if (GetType(q.data[1])==T_INT)
                        value=*((int*)op1);
                      else
                        value=*((double*)op1);
                     sprintf(l,"Variable %s: %12.4f \n",q.name[1],value);
                     messages << l;
                   }
                   break;
     case T_JMPF: if ( *((int*)GetPointer(q.data[1]))==0)
                    PC=q.data[3]-1;
                  break;
     case T_JMPT: if ( *((int*)GetPointer(q.data[1])))
                    PC=q.data[3]-1;
                  break;
     case T_OR:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[3])==T_INT)
                        *((int*)dest)=(*((int*)op1)) || (*((int*)op2));
                    }
                    break;
     case T_AND:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[3])==T_INT)
                        *((int*)dest)=(*((int*)op1)) && (*((int*)op2));
                    }
                    break;
     case T_GT:     {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) > (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) > (*((double*)op2));
                    }
                    break;
     case T_GTEQ:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) >= (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) >= (*((double*)op2));
                    }
                    break;
     case T_LW:     {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) < (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) < (*((double*)op2));
                    }
                    break;
     case T_LWEQ:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) <= (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) <= (*((double*)op2));
                    }
                    break;
     case T_EQ:     {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) == (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) == (*((double*)op2));
                    }
                    break;
     case T_NOTEQ:  {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) != (*((int*)op2));
                       else
                        *((int*)dest)=(*((double*)op1)) != (*((double*)op2));
                    }
                    break;
     case T_ASSIGN: {
                      op1=GetPointer(q.data[1]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[3])==T_INT)
                        *((int*)dest)=*((int*)op1);
                       else
                        *((double*)dest)=*((double*)op1);
                    }
                    break;
     case T_NOT:   {
                      op1=GetPointer(q.data[1]);
                      if (op1==NULL)
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)op1)= -1 * ( *((int*)op1) );
                       else
                        *((double*)op1)= -1.0 * ( *((double*)op1) );
                    }
                    break;
     case T_PLUS:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) + (*((int*)op2));
                       else
                        *((double*)dest)=(*((double*)op1)) + (*((double*)op2));
                    }
                    break;
     case T_MINUS:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) - (*((int*)op2));
                       else
                        *((double*)dest)=(*((double*)op1)) - (*((double*)op2));
                    }
                    break;
     case T_TIMES:  {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) * (*((int*)op2));
                       else
                        *((double*)dest)=(*((double*)op1)) * (*((double*)op2));
                    }
                    break;
     case T_OVER:   {
                      bool flag=false;
                      flag=((q.data[2]==T_INT) || (q.data[2]==T_NUM_INT));
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                       else
                        if (flag)
                          flag=((*((int*)op2))==0);
                         else
                          flag=((*((double*)op2))==0);
                       if (flag)
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "DIVIDE BY ZERO <EXCEPTION> at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) / (*((int*)op2));
                       else
                        *((double*)dest)=(*((double*)op1)) / (*((double*)op2));
                    }
                    break;
     case T_MOD:   {
                      op1=GetPointer(q.data[1]);
                      op2=GetPointer(q.data[2]);
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (op2==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if ((*((int*)op2))==0)
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "DIVIDE BY ZERO <EXCEPTION> at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      if (GetType(q.data[1])==T_INT)
                        *((int*)dest)=(*((int*)op1)) % (*((int*)op2));
                       else
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "ILLEGAL OPERATION at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                    }
                    break;
     case T_REAL2INT:{
                      op1=GetPointer(q.data[1]); // A Constant
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      *((int*)dest)=(int)(*((double*)op1));
                    }
                    break;
     case T_INT2REAL:{
                      op1=GetPointer(q.data[1]); // A Constant
                      dest=GetPointer(q.data[3]);
                      if ((op1==NULL) || (dest==NULL))
                        {
                           char l[20];
                           _itoa(PC,l,10);
                           messages << "UNAVAILABLE ADDRESS TRAP at line ";
                           messages << l;
                           messages << "\n";
                           return;
                        }
                      *((double*)dest)=(double)(*((int*)op1));
                    }
                    break;
   }
}

void* VirtualMachine::GetPointer(int address)
{
  switch(address/MEM_SIZE)
    {
      case 0: return &Constant->ints[address%MEM_SIZE];
      case 1: return &Constant->reals[address%MEM_SIZE];
      case 2: return &Global->ints[address%MEM_SIZE];
      case 3: return &Global->reals[address%MEM_SIZE];
      case 6: return &Temporal->ints[address%MEM_SIZE];
      case 7: return &Temporal->reals[address%MEM_SIZE];
//      case 4:
//      case 5:
    }
  messages << "TRAP! UNDEFINED ADDRESS!!\n";
  return NULL;
}

// Copies from Entity to Memory
void VirtualMachine::LoadEntityInfo()
{
  // Find entities in global table
  LocalVariableTable *table=SymTable->Search("GLOBAL")->local;
  LoadEntityInfo(table->root);
}

// Copies from Memory to Entity
void VirtualMachine::SaveEntityInfo()
{
  // Find entities in global table
  LocalVariableTable *table=SymTable->Search("GLOBAL")->local;
  SaveEntityInfo(table->root);
}

// Copies from Entity to Memory
void VirtualMachine::LoadEntityInfo(LV_Entry *v)
{
  if (v==NULL) return;
  LoadEntityInfo(v->left);
  LoadEntityInfo(v->right);
  if (v->type==T_ENTITY_VECTOR)
    {
      Vector3d value=World::current_world->GetEntity(v->name)->center;
      Global->reals[v->address-3*MEM_SIZE]=value.x;
      Global->reals[v->address-3*MEM_SIZE+1]=value.y;
      Global->reals[v->address-3*MEM_SIZE+2]=value.z;
    }
}

// Copies from Memory to Entity
void VirtualMachine::SaveEntityInfo(LV_Entry *v)
{
  if (v==NULL) return;
  SaveEntityInfo(v->left);
  SaveEntityInfo(v->right);
  if (v->type==T_ENTITY_VECTOR)
    {
      World::current_world->GetEntity(v->name)->center=Vector3d(
                                  Global->reals[v->address-3*MEM_SIZE],
                                  Global->reals[v->address-3*MEM_SIZE+1],
                                  Global->reals[v->address-3*MEM_SIZE+2]
                                                               );
    }
}

// One Statement at a time
void VirtualMachine::Step()
{
  // If no program in memory, halt
  if (PC<0)
    return;
  // Retrieve First Quadruple
  Quad q;
  do
  {
    q=ICode->data[PC];
    Execute(q);
    PC++;
    q=ICode->data[PC];
  } while (!q.start);
}


