// Quadruples
// (C) 1999 by A'r-Ci-O'oh

#include "quad.h"

Quad::Quad()
{
  for (int i=0; i<4; i++)
    {
       type[i]=Q_NONE;
       strcpy(name[i],"");
       data[i]=-1;
    }
  start=START;
  START=false;
}

Quad::~Quad()
{
  for (int i=0; i<4; i++)
    {
       type[i]=Q_NONE;
       strcpy(name[i],"");
       data[i]=-1;
    }
}

void Quad::Set(int field,QuadType typed, const char *string,int dat)
{
   if (field>3) return;
    else if (field<0) return;
   type[field]=typed;
   strcpy(name[field],string);
   data[field]=dat;
}

void Quad::Write(FILE *f)
{
   for (int i=0; i<4; i++)
     {
        fwrite(&type[i],sizeof(QuadType),1,f);
        fwrite(&name[i],(MAX_TOKEN_LENGTH+1)*sizeof(char),1,f);
        fwrite(&data[i],sizeof(int),1,f);
     }
}

// -------------------------------------------------------------------
// Simple Quadruple List

QuadList::QuadList()
{
  size=0;
  data=NULL;
}

QuadList::~QuadList()
{
  if (data)
    free(data);
}

void QuadList::PasteQuad(Quad q)
{
  if (data==NULL)
    {
       data=(Quad*)malloc(sizeof(Quad));
       memcpy(data,&q,sizeof(Quad));
       size=1;
    }
   else
     {
       Quad *old=data;
       size++;
       data=(Quad*)malloc(size*sizeof(Quad));
       memcpy(data,old,(size-1)*sizeof(Quad));
       free(old);
       memcpy(&data[size-1],&q,sizeof(Quad));
     }
}

void QuadList::Dump()
{
  FILE *f=fopen("icode.lst","w");
  FILE *g=fopen("icode.out","wb");
  // Headers
  fprintf(f,"Trohs Scripting Language Compiler\n");
  fprintf(f,"Generated Intermediate Code Quadruples\n");
  fprintf(f,"--------------------------------------\n\n");
  const char *zx="TROHS3D_BIN";
  fwrite(&zx[0],11,1,g);
  for (int i=0; i<size; i++)
    {
      Quad q=data[i];
      // Binary ICode
      q.Write(g);
      // ICode List
      fprintf(f,"%d: ",i);
      fprintf(f," %s,",q.name[0]);
      fprintf(f," %s,",q.name[1]);
      fprintf(f," %s,",q.name[2]);
      fprintf(f," %s\n",q.name[3]);
      fprintf(f,"      <",i);
      // Data/Address
      fprintf(f," %d,",q.data[0]);
      fprintf(f," %d,",q.data[1]);
      fprintf(f," %d,",q.data[2]);
      fprintf(f," %d>\n\n",q.data[3]);
    }
  fclose(f);
  fclose(g);
}

void QuadList::FillAddress(int quadnumber,int label)
{
   data[quadnumber].data[3]=label;
}

QuadList *ICode=NULL;
bool START=false;


