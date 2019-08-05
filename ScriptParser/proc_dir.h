// Procedure Directory and Address Manager
// (C) 1999 by A'r-Ci-O'oh

#ifndef __PROC_DIR_H__
#define __PROC_DIR_H__

#include "../Scanner/script.h"
#include "../Scanner/scanner.h"

class VirtualMachine;

// A Local Variable Entry
struct LV_Entry
{
  LV_Entry *left,*right;                // Pointers to next variables
  char name[MAX_TOKEN_LENGTH+1];        // Identifier or Constant Name
  TokenType type;                       // Type
  int address;                          // Virtual Address
  void *data;                           // Actual Data!
  LV_Entry(const char *n,TokenType t,int addr)
    {
       strcpy(name,n);
       type=t;
       left=right=NULL;
       address=addr;
    }
};


// Local Symbol table associated with a module
class LocalVariableTable
{
  friend class VirtualMachine;
  protected:
    LV_Entry *root;
    void Clear(LV_Entry *r);
    LV_Entry* Search(int address,LV_Entry *r,bool showerror=false);
  public:
    LocalVariableTable();
    ~LocalVariableTable();
    // Insert a variable. Address is automatically assigned on next avail
    LV_Entry* Insert(const char *name,TokenType t);
    // Search for a variable by name
    LV_Entry* Search(const char *name,bool showerror=false);
    // Search for a variable by address
    LV_Entry* Search(int address,bool showerror=false);
};

// An Entry in the Procedure Directory
class PD_Entry
{
 public:
   LocalVariableTable *local;            // Ptr to Local Variables
   char name[MAX_TOKEN_LENGTH+1];        // Procedure name
   int MaxInt,MaxReal;                   // Max # of variables
   PD_Entry *left,*right;
   PD_Entry() { local=NULL; left=right=NULL; MaxInt=MaxReal=0; };
   PD_Entry(const char *n) { strcpy(name,n); local=NULL; left=right=NULL; MaxInt=MaxReal=0; };
   ~PD_Entry() { delete local; };
   void SetMax(int sint,int sreal) { MaxInt=sint; MaxReal=sreal; };
};

// --------------------------------------------------------------------
class ProcedureDirectory
{
  friend class VirtualMachine;
  public:
    ProcedureDirectory();
    ~ProcedureDirectory();
    // Insert a Module
    PD_Entry* Insert(const char *name);
    // Search for a Module
    PD_Entry* Search(const char *name,bool showerror=false);
    // Returns a LV_Entry searching in ALL Proc Dir
    LV_Entry* GetEntry(const char *name);
  protected:
    PD_Entry *root;
    void Clear(PD_Entry *r);
    LV_Entry* SearchForLV(const char *name,PD_Entry *where);
};

extern ProcedureDirectory *SymTable;

#endif
