// Procedure Directory
// (C) 1999 by A'r-Ci-O'oh

#include "../Scanner/script.h"
#include "proc_dir.h"
#include "address.h"

LocalVariableTable::LocalVariableTable()
{
  root=NULL;
}

LocalVariableTable::~LocalVariableTable()
{
  Clear(root);
}

void LocalVariableTable::Clear(LV_Entry *r)
{
   if (r==NULL)
     return;
   Clear(r->left);
   Clear(r->right);
   delete r;
   r=NULL;
}

LV_Entry* LocalVariableTable::Insert(const char *name,TokenType t)
{
   LV_Entry *father=NULL,*son=root;
   int val=-10000;
   while (son!=NULL)
     {
       val=strcmp(name,son->name);
       if (val==0)
         {
           // Name is already Entered!
           char s[100]="Identifier ";
           strcat(s,tokenstring);
           strcat(s," Already Declared ");
           SemanticError(s);
           return son;
         }
       father=son;
       if (val<0)
         son=son->left;
        else
         son=son->right;
     }
  // Create a new node for son and paste it to father
  if (val==-10000)
    {
       // Empty Tree
       root=new LV_Entry(name,t,GetAddress(t));
       return root;
    }
  son=new LV_Entry(name,t,GetAddress(t));
  if (val<0)
    father->left=son;
   else
    father->right=son;
  return son;
}

LV_Entry* LocalVariableTable::Search(const char *name,bool showerror)
{
   LV_Entry *r=root;
   while (r!=NULL)
     {
       int val=strcmp(name,r->name);
       if (val==0)
         return r; // Found!
       if (val<0)
         r=r->left;
        else
         r=r->right;
     }
   if (showerror)
     {
           // Name was not found!
           char s[100]="Identifier ";
           strcat(s,tokenstring);
           strcat(s," was not Declared Previously!");
           SemanticError(s);
     }
   return NULL;
}

// Search for a variable by address
LV_Entry* LocalVariableTable::Search(int address,bool showerror)
{
   LV_Entry *r=Search(address,root,showerror);
   // Name was not found!
   if (!r)
     if (showerror)
           SemanticError("INTERNAL ERROR: Address Not Found!");
   return r;
}

LV_Entry* LocalVariableTable::Search(int address,LV_Entry *r,bool showerror)
{
   LV_Entry *dummy;
   if (r!=NULL)
     {
       if (r->address==address)
         return r; // Found!
       dummy=Search(address,r->left,showerror);
       if (dummy!=NULL)
         return dummy;
       dummy=Search(address,r->right,showerror);
       if (dummy!=NULL)
         return dummy;
     }
   return NULL;
}

// --------------------------------------------------
ProcedureDirectory::ProcedureDirectory()
{
  root=NULL;
}

ProcedureDirectory::~ProcedureDirectory()
{
  Clear(root);
}


PD_Entry* ProcedureDirectory::Insert(const char *name)
{
   PD_Entry *father=NULL,*son=root;
   int val=-10000;
   while (son!=NULL)
     {
       val=strcmp(name,son->name);
       if (val==0)
         {
           // Name is already Entered!
           char s[100]="Module ";
           strcat(s,tokenstring);
           strcat(s," Already Declared ");
           SemanticError(s);
           return son;
         }
       father=son;
       if (val<0)
         son=son->left;
        else
         son=son->right;
     }
  if (val==-10000)
    {
       // Empty Tree
       root=new PD_Entry(name);
       root->local=new LocalVariableTable();
       return root;
    }
  // Create a new node for son and paste it to father
  son=new PD_Entry(name);
  son->local=new LocalVariableTable();
  if (val<0)
    father->left=son;
   else
    father->right=son;
  return son;
}

void ProcedureDirectory::Clear(PD_Entry *r)
{
   if (r==NULL)
     return;
   Clear(r->left);
   Clear(r->right);
   delete r;
   r=NULL;
}

PD_Entry* ProcedureDirectory::Search(const char *name,bool showerror)
{
   PD_Entry *r=root;
   while (r!=NULL)
     {
       int val=strcmp(name,r->name);
       if (val==0)
         return r;
       if (val<0)
         r=r->left;
        else
         r=r->right;
     }
   if (showerror)
     {
           // Name was not found!
           char s[100]="Module ";
           strcat(s,tokenstring);
           strcat(s," was not Declared Previously!");
           SemanticError(s);
     }
   return NULL;
}

LV_Entry* ProcedureDirectory::GetEntry(const char *name)
{
   return SearchForLV(name,root);
}

LV_Entry* ProcedureDirectory::SearchForLV(const char *name,PD_Entry *where)
{
  if (where==NULL)
    return NULL;
  LV_Entry *r=where->local->Search(name);
  if (r==NULL)
    {
      r=SearchForLV(name,where->left);
      if (r==NULL)
        {
          r=SearchForLV(name,where->left);
          if (r!=NULL)
            return r;
        }
       else return r;
    }
  return r;
}


ProcedureDirectory *SymTable=NULL;


