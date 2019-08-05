// Token Defs
// (C) 1999 by A'r-Ci-O'oh

#ifndef __TOKENS_H__
#define __TOKENS_H__


enum TokenType
{
  T_EOF,T_ERROR,
  T_ID,T_NUM_INT,T_NUM_REAL,
  T_EQ,T_NOTEQ,
  T_LWEQ,T_GTEQ,
  T_ASSIGN,T_NOT,
  T_LW,T_GT,
  T_DOT,T_SEMICOLON,T_COLON,T_COMMA,
  T_LP,T_RP,
  T_LB,T_RB,
  T_AND,T_OR,
  T_PLUSPLUS,T_MINUSMINUS,
  T_PLUS,T_MINUS,T_TIMES,T_OVER,T_MOD,
  T_IF,T_ELSE,T_WHILE,T_REPEAT,T_UNTIL,
  T_OBJECTS,T_STRING,

  // World File
  T_WORLD,T_SECTOR,T_POLYGON,T_VERTEX,T_CAMERA,
  T_TYPE,T_VERTICES,T_SOLID,T_WIREFRAME,T_TEXTURE,T_COLOR,

  // Script File
  T_SCRIPT,T_INIT,T_VARS,T_MAIN,
  T_INT,T_REAL,T_VECTOR,
  T_ENTITY,T_CENTER,
  T_PRINT,

  // This are included here for compatibility, but are meaningful only
  // for the Trohs Virtual Machine!
  T_JMP,T_JMPF,T_JMPT,
  T_INT2REAL,T_REAL2INT,
  T_ENTITY_VECTOR
};

#endif