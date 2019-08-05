// Console Class
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "trohs3d.h"

// -------------------------------------------------------------------------
// This is information that will be displayed on each frame
// This info is displayed on Renderer Screen
// Displays at most MAX_LAST_LINES-1 in the screen
#define MAX_LAST_LINES 7

class MessageConsole
{
  public:
    MessageConsole();
    ~MessageConsole() {};
    // Add a text to Console
    MessageConsole& operator<<(const char *line);
    // Dump Console to where
    MessageConsole& operator>>(BITMAP *where);
    int MaxLines;
  protected:
    bool lasteol;
    char text[MAX_LAST_LINES][80];
    void Shift();
};

// ------------------------------------------------------------
// Console. This is another 'program' in itself. It is a simple
// Command Interpreter
class Console
{
  public:
    Console();
    ~Console() {};
    // This method runs the command console and waits and executes commands
    void Run();
  protected:
    // # of lines in current screen
    int max;
    char text[120][80];
    bool ProcessKey(int key);
    void Shift();
    void Prompt();
    void Cls();
    int ParseCommand();
    bool ExecuteCommand(int command);
    // Position & current line, cursor timer count & on/off flag
    int x;
    int line;
    int count;
    bool cursor;
};

extern MessageConsole messages;
extern Console console;


#endif

