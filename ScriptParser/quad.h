// QUadruple Generator
// (C) 1999 by A'r-Ci-O'oh

#ifndef __QUAD_H__
#define __QUAD_H__

#include "../Scanner/script.h"
#include "../Scanner/tokens.h"

// A Quadruple item can be Nothing, a operation, a variable address
typedef enum
{
  Q_NONE,
  Q_EMPTY,
  Q_OPER,   // Operation
  Q_ADDR,   // Address of Symbol
  Q_LABEL   // Quad Label
} QuadType;

class Quad
{
  public:
    Quad();
    ~Quad();
    void Set(int field,QuadType type, const char *string,int dat);
    QuadType type[4];
    char name[4][MAX_TOKEN_LENGTH+1];
    int data[4];
    bool start;
    void Write(FILE *f);
};

// -------------------------------------------------------------------
// Simple Quadruple List
class VirtualMachine;
class Console;

class QuadList
{
  friend VirtualMachine;
  friend Console;
  public:
    QuadList();
    ~QuadList();
    void PasteQuad(Quad q);
    void Dump();
    void FillAddress(int quadnumber,int data);
    int GetAddress() { return size-1; };
  protected:
    int size;
    Quad *data;
};

extern QuadList *ICode;
extern bool START;

#endif