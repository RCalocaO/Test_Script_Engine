// Virtual Machine
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __VIRTUALMACHINE_H__
#define __VIRTUALMACHINE_H__

#include "../trohs3d.h"
class Quad;
class Console;
struct LV_Entry;

// Memory!
struct MemoryTable
{
   int* ints;
   double* reals;
   MemoryTable* next;
   MemoryTable()
     { ints=NULL; reals=NULL; next=NULL; };
};

class VirtualMachine
{
  friend class Console;
  public:
    VirtualMachine();
    ~VirtualMachine();
    void Reset();
    bool Load();
    void Step();
    void ICodeStep();
  protected:
    int PC;
    void Execute(Quad &q);
    void ExecuteOperation(Quad &q);
    void* GetPointer(int address);
    MemoryTable *Global;
    MemoryTable *Constant;
    MemoryTable *Local;
    MemoryTable *Temporal;
    void LoadEntityInfo();
    void SaveEntityInfo();
    void LoadEntityInfo(LV_Entry *root);
    void SaveEntityInfo(LV_Entry *root);
};


extern VirtualMachine Trohs3dVM;

#endif
