// Address Handler
// (C) 1999 by A'r-Ci-O'oh

#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include "../Scanner/script.h"
#include "../Scanner/tokens.h"
#include "stacks.h"

// OK, let's define our memory layout
/*
   MEM_SIZE
   Constants:
     Int:              0..MEM_SIZE-1
     Real:      MEM_SIZE..2*MEM_SIZE-1

   Global:
     Int:     2*MEM_SIZE..3*MEM_SIZE-1
     Real:    3*MEM_SIZE..4*MEM_SIZE-1

   Local:
     Int:     4*MEM_SIZE..5*MEM_SIZE-1
     Real:    5*MEM_SIZE..6*MEM_SIZE-1

   Temporal:
     Int:     6*MEM_SIZE..7*MEM_SIZE-1
     Real:    7*MEM_SIZE..8*MEM_SIZE-1
*/

// # of elements in a memory zone
#define MEM_SIZE 1000

// True if Parsing Local Variables, False if Global Variables
extern bool LocalZone;

// Resets local zone counters
extern void ResetZone();

// Gets next avail address for type
extern int GetAddress(TokenType type);

// "Allocates" Information for a new Temporal of type
extern TokenItem GetNextTemporal(TokenType type);


// Returns the type according to an address (int or real)
extern TokenType GetType(int address);

#endif

