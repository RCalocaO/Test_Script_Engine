// Scanner
// (C) 1999 by A'r-Ci-O'oh

#include "scanner.h"

// Current Token
char tokenstring[MAX_TOKEN_LENGTH+1];

// Max Length of current line
#define BUFLEN 256
static char linebuf[BUFLEN];    // Current line
static int linepos=0;           // Current pos @ linebuf
static int bufsize=0;           // Actual size of current line

// Get Next Character
static char GetNext()
{
  // If new line, fetch it!
  if (!(linepos<bufsize))
    {
       lineno++;
       // Fetch new Line
       if (fgets(linebuf,BUFLEN-1,script_in))
         {
            // If echo to Listing...
//            if (PrintSource)
//              fprintf(script_list,"%4d: %s",lineno,linebuf);
            bufsize=strlen(linebuf);
            linepos=0;
            return linebuf[linepos++];
         }
        else
         return EOF;
    }
   else return linebuf[linepos++];
}

static void UnGetNext()
{
   linepos--;
}

// Table for Reserved Words
static struct
{
  const char *str;
  TokenType tok;
} reservedwords[]=
    {
     // World File
     {"WORLD",T_WORLD},
     {"SECTOR",T_SECTOR},
     {"POLYGON",T_POLYGON},
     {"VERTEX",T_VERTEX},
     {"TYPE",T_TYPE},
     {"VERTICES",T_VERTICES},
     {"SOLID",T_SOLID},
     {"WIREFRAME",T_WIREFRAME},
     {"TEXTURE",T_TEXTURE},
     {"COLOR",T_COLOR},
     {"CAMERA",T_CAMERA},
     // Script File
     {"SCRIPT",T_SCRIPT},
     {"INIT",T_INIT},
     {"VARS",T_VARS},
     {"ENTITY",T_ENTITY},
     {"CENTER",T_CENTER},
     {"PRINT",T_PRINT},

     {"INT",T_INT},
     {"REAL",T_REAL},
     {"VECTOR",T_VECTOR},
     {"IF",T_IF},
     {"ELSE",T_ELSE},
     {"REPEAT",T_REPEAT},
     {"UNTIL",T_UNTIL},
     {"&&",T_AND},
     {"||",T_OR},
     {"WHILE",T_WHILE},
     {"OBJECTS",T_OBJECTS},
     {"MAIN",T_MAIN},
     {"VARS",T_VARS},
     {"",T_ERROR}
    };


// States in DFA of scanner
typedef enum
{
  S_START,S_COMMENT,S_NUM,S_ID,S_DONE,S_STRING,S_PLUS,S_MINUS,
  S_GT,S_LW,S_EQ,S_NOT
} StateType;

// Check if identifier is a reserved word
static TokenType ReservedLookUp(char *s)
{
   for (int i=0; strcmp(reservedwords[i].str,"")!=0; i++)
     {
       if (!strcmp(s,reservedwords[i].str))
         return reservedwords[i].tok;
     }
   return T_ID;
}

// Return Next Token in Source File
TokenType GetToken()
{
  int tokenstringindex=0;
  bool dot=false;
  TokenType currentoken;
  StateType state=S_START;
  bool copy=true;
  while (state!=S_DONE)
    {
       char c=GetNext();
       char cn;
       copy=true;
       switch(state)
         {
            case S_START:
                    if (c=='\"')
                      {
                        state=S_STRING;
                        copy=false;
                      }
                    else if (isdigit(c))
                      state=S_NUM;
                    else if ((isalpha(c)) || (c=='_') || (c=='&') || (c=='|'))
                      state=S_ID;
                    else if ((c==' ') || (c=='\t') || (c=='\r') || (c=='\n'))
                      copy=false;
                    else
                      {
                        state=S_DONE;
                        switch(c)
                          {
                             case EOF:
                                  copy=false;
                                  currentoken=T_EOF;
                                  break;
                             case '*':
                                  currentoken=T_TIMES;
                                  break;
                             case ';':
                                  currentoken=T_SEMICOLON;
                                  break;
                             case ':':
                                  currentoken=T_COLON;
                                  break;
                             case ',':
                                  currentoken=T_COMMA;
                                  break;
                             case '.':
                                  currentoken=T_DOT;
                                  break;
                             case '%':
                                  currentoken=T_MOD;
                                  break;
                             case '(':
                                  currentoken=T_LP;
                                  break;
                             case ')':
                                  currentoken=T_RP;
                                  break;
                             case '{':
                                  currentoken=T_LB;
                                  break;
                             case '}':
                                  currentoken=T_RB;
                                  break;
                             case '=':
                               state=S_EQ;
                               break;
                             case '!':
                               state=S_NOT;
                               break;
                             case '+':
                               state=S_PLUS;
                               break;
                             case '-':
                               state=S_MINUS;
                               break;
                             case '<':
                               state=S_LW;
                               break;
                             case '>':
                               state=S_GT;
                               break;
                             case '/':
                               cn=GetNext(); UnGetNext();
                               if (cn=='/')
                                 {
                                   copy=false;
                                   state=S_COMMENT;
                                 }
                                else
                                 currentoken=T_OVER;
                               break;
                          }
                      }
                    break;
            case S_PLUS:
                 state=S_DONE;
                 if (c=='+')
                   currentoken=T_PLUSPLUS;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_PLUS;
                   }
                 break;
            case S_MINUS:
                 state=S_DONE;
                 if (c=='-')
                   currentoken=T_MINUSMINUS;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_MINUS;
                   }
                 break;
            case S_NOT:
                 state=S_DONE;
                 if (c=='=')
                   currentoken=T_NOTEQ;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_NOT;
                   }
                 break;
            case S_GT:
                 state=S_DONE;
                 if (c=='=')
                   currentoken=T_GTEQ;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_GT;
                   }
                 break;
            case S_LW:
                 state=S_DONE;
                 if (c=='=')
                   currentoken=T_LWEQ;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_LW;
                   }
                 break;
            case S_EQ:
                 state=S_DONE;
                 if (c=='=')
                   currentoken=T_EQ;
                  else
                   {
                     UnGetNext();
                     copy=false;
                     currentoken=T_ASSIGN;
                   }
                 break;
            case S_COMMENT:
                 copy=false;
                 if ((c=='\r') || (c=='\n')) state=S_START;
                 break;
            case S_STRING:
                 if (c=='\"')
                   {
                     state=S_DONE;
                     copy=false;
                     currentoken=T_STRING;
                   }
                 break;
            case S_NUM:
                 if ((dot) && (c=='.'))
                       {
                         state=S_DONE;
                         currentoken=T_ERROR;
                       }
                      else
                       if (c=='.') dot=true;
                 if ((c!='.') && (!isdigit(c)))
                   {
                     UnGetNext();
                     copy=false;
                     state=S_DONE;
                     if (dot==true)
                       currentoken=T_NUM_REAL;
                      else
                       currentoken=T_NUM_INT;
                   }
                 break;
            case S_ID:
                 if (!(isalpha(c)) && (!isdigit(c)) && (c!='_')  && (c!='&') && (c!='|'))
                    {
                      UnGetNext();
                      copy=false;
                      state=S_DONE;
                      currentoken=T_ID;
                    }
                 break;
            case S_DONE:
            default:
                 fprintf(script_list,"Scanner Bug: State= %d\n",state);
                 state=S_DONE;
                 currentoken=T_ERROR;
                 break;
         }
       if ((copy) && (tokenstringindex<=MAX_TOKEN_LENGTH))
         tokenstring[tokenstringindex++]=c;
       if (state==S_DONE)
         {
           tokenstring[tokenstringindex]='\0';
           tokenstringindex=0;
           if (currentoken==T_ID)
             currentoken=ReservedLookUp(tokenstring);
         }
    }
//  if (TraceScanner)
//    {
//       fprintf(script_list,"      %d: ",lineno);
//       PrintToken(currentoken,tokenstring);
//    }
  return currentoken;
}


