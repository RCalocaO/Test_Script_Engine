// Scanner
// (C) 1999 by A'r-Ci-O'oh

#ifndef __SCAN_H__
#define __SCAN_H__

#include "script.h"
#include "tokens.h"

#define MAX_TOKEN_LENGTH 64

extern char tokenstring[MAX_TOKEN_LENGTH+1];

extern TokenType GetToken();
extern void SemanticError(const char *msg);
extern void SyntaxError(const char *msg);

#endif

