// Console Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "console.h"
#include "system.h"
#include "ScriptParser/proc_dir.h"
#include "ScriptParser/address.h"
#include "VirtualMachine/virtualmachine.h"
#include "ScriptParser/quad.h"
#include <dos.h>

#define CONSOLE_BAD     -1
#define CONSOLE_IGNORE  0
#define CONSOLE_QUIT    1
#define CONSOLE_CLS     2
#define CONSOLE_HELP    3
#define CONSOLE_ABOUT   4
#define CONSOLE_RESET   5
#define CONSOLE_STEP    6
#define CONSOLE_DUMP    7
#define CONSOLE_TRACE   8

Console console;
MessageConsole messages;

MessageConsole::MessageConsole()
{
  MaxLines=MAX_LAST_LINES;
  for (int i=0; i<MAX_LAST_LINES; i++) memset(text[i],80,0);
  lasteol=false;
}

MessageConsole& MessageConsole::operator<<(const char *line)
{
  if (lasteol)
   {
     Shift();
     lasteol=false;
     memset(text[MaxLines-1],80,0);
   }
  strcat(text[MaxLines-1],line);
  if (strstr(line,"\n"))
    lasteol=true;
  return *this;
}

MessageConsole& MessageConsole::operator>>(BITMAP *where)
{
#if 0
  int ypos=-10;
  for (int i=0; i<MaxLines; i++)
    if (text[i][0])
      textprintf(where,font,0,(ypos+=10),15,text[i]);
#endif
  return *this;
}

// Move all lines one down
void MessageConsole::Shift()
{
  for (int i=0; i<MaxLines-1; i++) memcpy(text[i],text[i+1],80);
  strcpy(text[MaxLines-1],"");
}

// ----------------------------------------------------------------------------
// cls is used so that console is unable to use the F3 key when clearing screen
// repeat is used to save last line
bool cls;
static char repeat[80]="trohs3d> ";

Console::Console()
{
  for (int i=0; i<120; i++) memset(text[i],80,0);
  max=0;
  line=0;
  Cls();
  Prompt();
}

// Tests if Scrolling is needed
void Console::Shift()
{
  if (line>max-1)
    {
      line=max-1;
      for (int i=0; i<line; i++)
        memcpy(text[i],text[i+1],80);
      //Prompt();
    }
}

void Console::Run()
{
#if 0
  // Wait till user releases key!
  cursor=true;
  while (key[KEY_TAB]) ;
  readkey();
  delay(50);
  bool keywaiting=false;
  int dummy;
  do
  {
    if (!keypressed())
      {
        keywaiting=false;
        break;
      }
    dummy=readkey();
  } while (dummy==KEY_TAB);
  if (keywaiting) simulate_keypress(dummy);
  max=SCREEN_H/10;
  do
   {
     // Clear Last Cursor
     text[line][x]=0;
     // Get New KeyPressed
     if (keypressed())
       if (ProcessKey(readkey()))
         {
           // Wait 'till user releases key!
           while (key[KEY_TAB]) ;
           delay(50);
           return;
         }
     // Animate Cursor
     if (!count) cursor=!cursor;
     count=(count+1)%15;
     if (cursor)
       {
         text[line][x]='_';
         text[line][x+1]=0;
       }
      else
         text[line][x]=0;
     // Update text to video
     clear(video);
     int ypos=-10;
     for (int i=0; i<max; i++)
        textprintf(video,font,0,(ypos+=10),15,text[i]);
     blit(video,screen,0,0,0,0,SCREEN_W,SCREEN_H);
   } while (true);
#endif
}

// Reads a character from keyboard. Returns true if tab pressed
bool Console::ProcessKey(int key)
{
#if 0
  static int oldx=9;
  int msbkey=key>>8;
  if (msbkey==KEY_TAB)
    return true;
  if (msbkey==KEY_BACKSPACE)
    {
      text[line][x--]=0;
      if (x<9) x=9;
      return false;
    }
  if (msbkey==KEY_ESC)
    {
      Prompt();
      Shift();
      return false;
    }
  if (msbkey==KEY_F3)
   if (!cls)
    {
      if (line>0)
        strcpy(text[line],repeat);
      x=oldx;
      return false;
    }
  if (msbkey==KEY_ENTER)
    {
      strcpy(repeat,text[line]);
      line++;
      oldx=x;
      Shift();
      int command=ParseCommand();
      Prompt();
      if (cls) cls=false;
      return ExecuteCommand(command);
      // return false;
    }
  // Accept one character and display it
  key&=0xff;
  if (((key>='A') && (key<='Z')) || ((key>='0') && (key<='9')) ||
       (key==' ') || (key=='(') || (key==')') || (key=='-') || (key=='.') ||
       (key=='\\') || (key=='!') || (key=='=') || (key=='?') || (key==',') ||
       (key=='*') || (key=='+') || (key=='-') || (key==',') || (key=='/') ||
      ((key>='a') && (key<='z')))
    text[line][x++]=key;
#endif
  return false;
}

void Console::Prompt()
{
  memset(text[line],80,0);
  strcpy(text[line],"trohs3d> ");
  x=9;
}

void Console::Cls()
{
#if 0
  for (int i=0; i<120; i++) memset(&text[i][0],0,80);
  line=0;
  max=SCREEN_H/10;
  strcpy(text[line++],"Trohs 3d Engine Console v2.0");
  strcpy(text[line++],"(C) 1999 by R. Caloca O.");
  strcpy(text[line++],"----------------------------");
  cls=true;
  Prompt();
#endif
}

int Console::ParseCommand()
{
  if (x==9) return CONSOLE_IGNORE;
  if (!strcmp(&text[line-1][9],"cls"))
    return CONSOLE_CLS;
  if (!strcmp(&text[line-1][9],"quit"))
    return CONSOLE_QUIT;
  if (!strcmp(&text[line-1][9],"help"))
    return CONSOLE_HELP;
  if (!strcmp(&text[line-1][9],"?"))
    return CONSOLE_HELP;
  if (!strcmp(&text[line-1][9],"about"))
    return CONSOLE_ABOUT;
  if (!strcmp(&text[line-1][9],"reset"))
    return CONSOLE_RESET;
  if (!strcmp(&text[line-1][9],"step"))
    return CONSOLE_STEP;
  if (!strcmp(&text[line-1][9],"trace"))
    return CONSOLE_TRACE;
  if (!strncmp(&text[line-1][9],"dump ",4))
    return CONSOLE_DUMP;
  return CONSOLE_BAD;
}

bool Console::ExecuteCommand(int command)
{
  switch(command)
    {
      case CONSOLE_IGNORE: break;
      case CONSOLE_CLS:    Cls();
                           break;
      case CONSOLE_QUIT:   ShutDown=true;
                           return true;
      case CONSOLE_HELP:   Shift();
                           strcpy(text[line++],"         Command List:");
                           Shift();
                           strcpy(text[line++],"              'about'");
                           strcpy(text[line++],"              'cls'");
                           strcpy(text[line++],"              'reset'");
                           strcpy(text[line++],"              'step'");
                           strcpy(text[line++],"              'trace'");
                           strcpy(text[line++],"              'dump'");
                           strcpy(text[line++],"              'quit'");
                           strcpy(text[line++],"              'help' or '?'");
                           Shift();
                           Prompt();
                           break;
      case CONSOLE_ABOUT:  Shift();
                           strcpy(text[line++],"         Trohs 3d Engine Command Console v1.0");
                           Shift();
                           strcpy(text[line++],"         (C) 1999 by R. Caloca O.");
                           Shift();
                           strcpy(text[line++],"              Command Console for Trohs 3d Engine Project");
                           Shift();
                           strcpy(text[line++],"              Type help for other commands.");
                           Shift();
                           Prompt();
                           break;
      case CONSOLE_BAD:    Shift();
                           strcpy(text[line++],">>>>>>>>>>>> ERROR!!!");
                           Shift();
                           strcpy(text[line++],"         Inexistent Command or Switch. Try using help");
                           Shift();
                           Prompt();
                           break;
      case CONSOLE_RESET:  Shift();
                           strcpy(text[line++],"         Reseting Virtual Machine.");
                           Trohs3dVM.Reset();
                           Shift();
                           Prompt();
                           break;
      case CONSOLE_STEP:   {
                             Shift();
                             strcpy(text[line++],"         Virtual Machine Executing Quadruple:");
                             Shift();
                             sprintf(text[line++],"        %d:",Trohs3dVM.PC);
                             Shift();
                             sprintf(text[line++],"              Name: < %s, %s, %s, %s >",ICode->data[Trohs3dVM.PC].name[0],ICode->data[Trohs3dVM.PC].name[1],ICode->data[Trohs3dVM.PC].name[2],ICode->data[Trohs3dVM.PC].name[3]);
                             Shift();
                             sprintf(text[line++],"              Type: ( %d, %d, %d, %d )",ICode->data[Trohs3dVM.PC].type[0],ICode->data[Trohs3dVM.PC].type[1],ICode->data[Trohs3dVM.PC].type[2],ICode->data[Trohs3dVM.PC].type[3]);
                             Shift();
                             sprintf(text[line++],"              Data: [ %d, %d, %d, %d ]",ICode->data[Trohs3dVM.PC].data[0],ICode->data[Trohs3dVM.PC].data[1],ICode->data[Trohs3dVM.PC].data[2],ICode->data[Trohs3dVM.PC].data[3]);
                             Shift();
                             Trohs3dVM.Step();
                             Shift();
                             Prompt();
                           }
                           break;
      case CONSOLE_TRACE:   {
                             Shift();
                             strcpy(text[line++],"         Virtual Machine Executing Quadruple:");
                             Shift();
                             sprintf(text[line++],"        %d:",Trohs3dVM.PC);
                             Shift();
                             sprintf(text[line++],"              Name: < %s, %s, %s, %s >",ICode->data[Trohs3dVM.PC].name[0],ICode->data[Trohs3dVM.PC].name[1],ICode->data[Trohs3dVM.PC].name[2],ICode->data[Trohs3dVM.PC].name[3]);
                             Shift();
                             sprintf(text[line++],"              Type: ( %d, %d, %d, %d )",ICode->data[Trohs3dVM.PC].type[0],ICode->data[Trohs3dVM.PC].type[1],ICode->data[Trohs3dVM.PC].type[2],ICode->data[Trohs3dVM.PC].type[3]);
                             Shift();
                             sprintf(text[line++],"              Data: [ %d, %d, %d, %d ]",ICode->data[Trohs3dVM.PC].data[0],ICode->data[Trohs3dVM.PC].data[1],ICode->data[Trohs3dVM.PC].data[2],ICode->data[Trohs3dVM.PC].data[3]);
                             Shift();
                             Trohs3dVM.ICodeStep();
                             Shift();
                             Prompt();
                           }
                           break;
      case CONSOLE_DUMP:   {
                             char temp[80];
                             strcpy(temp,&text[line-1][14]);
                             Shift();
                             strcpy(text[line],"         Dumping Variable ");
                             strcat(text[line],temp);
                             LV_Entry *block=SymTable->Search("GLOBAL")->local->Search(temp);
                             if (block==NULL)
                               if ((block=SymTable->Search("TEMPORAL")->local->Search(temp))==NULL)
                                 if ((block=SymTable->Search("CONSTANT")->local->Search(temp))==NULL)
                                   // Local!
                                   strcat(text[line++]," << VARIABLE NOT FOUND IN ANY BLOCK! >>");
                             if (block)
                               {
                                 // Now get current address and retrieve info!
                                   void *op1=Trohs3dVM.GetPointer(block->address);
                                   double value;
                                   if (GetType(block->address)==T_INT)
                                     value=(double) *((int*)op1);
                                    else
                                     value=*((double*)op1);
                                   sprintf(temp," %12.4f",value);
                                   strcat(text[line++],temp);
                               }
                             Shift();
                             Prompt();
                           }
                           break;
    }
  return false;
}



