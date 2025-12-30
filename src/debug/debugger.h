#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <stdbool.h>

/*
 * Minimal debugger for Mango68k bring-up
 */
class Debugger {
public:
    Debugger();
    ~Debugger();

    /* Called once per executed instruction */
    void onInstruction();

    bool singlestep;
    bool verbose;

private:
    void dumpRegisters();
    void waitForUser();
};

extern Debugger *gDebugger;

#endif /* __DEBUGGER_H__ */
