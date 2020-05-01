
/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs
 *  and could serve as the starting point for developing your first PIN tool
 */

#include <fstream>
#include <iostream>

#include "pin.H"
using std::cerr;
using std::endl;
using std::string;

/* ================================================================== */
// Global variables
/* ================================================================== */

UINT64 insCount = 0;    // number of dynamically executed instructions
UINT64 bblCount = 0;    // number of dynamically executed basic blocks
UINT64 threadCount = 0; // total number of threads, including main thread
ADDRINT retAddress = 0;
char buffer[135169];

std::ostream *out = &cerr;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "",
                            "specify file name for MyPinTool output");

KNOB<BOOL> KnobCount(
    KNOB_MODE_WRITEONCE, "pintool", "count", "1",
    "count instructions, basic blocks and threads in the application");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage() {
    cerr << "This tool prints out the number of dynamically executed " << endl
         << "instructions, basic blocks and threads in the application." << endl
         << endl;

    cerr << KNOB_BASE::StringKnobSummary() << endl;

    return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

/*!
 * Increase counter of threads in the application.
 * This function is called for every thread created by the application when it
 * is about to start running (including the root thread).
 * @param[in]   threadIndex     ID assigned by PIN to the new thread
 * @param[in]   ctxt            initial register state for the new thread
 * @param[in]   flags           thread creation flags (OS specific)
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddThreadStartFunction function call
 */
VOID ThreadStart(THREADID threadIndex, CONTEXT *ctxt, INT32 flags, VOID *v) {
    threadCount++;
}

VOID setReg(ADDRINT *regRef, UINT64 val) {
    // return;
    // fprintf(stderr, "original value: %ld\n", *regRef);
    // fprintf(stderr, "change reg to %ld\n", val);
    *regRef = val;
}

// Print syscall number and arguments
VOID SysBefore(ADDRINT ip, ADDRINT num, ADDRINT arg0, ADDRINT arg1,
               ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5) {
#if defined(TARGET_LINUX) && defined(TARGET_IA32)
    // On ia32 Linux, there are only 5 registers for passing system call
    // arguments, but mmap needs 6. For mmap on ia32, the first argument to the
    // system call is a pointer to an array of the 6 arguments
    if (num == SYS_mmap) {
        ADDRINT *mmapArgs = reinterpret_cast<ADDRINT *>(arg0);
        arg0 = mmapArgs[0];
        arg1 = mmapArgs[1];
        arg2 = mmapArgs[2];
        arg3 = mmapArgs[3];
        arg4 = mmapArgs[4];
        arg5 = mmapArgs[5];
    }
#endif

    fprintf(stderr,
            "syscall:0x%lx: %ld(0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx, 0x%lx)",
            (unsigned long)ip, (long)num, (unsigned long)arg0,
            (unsigned long)arg1, (unsigned long)arg2, (unsigned long)arg3,
            (unsigned long)arg4, (unsigned long)arg5);
}

VOID Routine(RTN rtn, VOID *v) {

    // The RTN goes away when the image is unloaded, so save it now
    // because we need it in the fini
    fprintf(stderr, RTN_Name(rtn).c_str());
    fprintf(stderr, "\n");
    // rc->_name = RTN_Name(rtn);
    // rc->_image = StripPath(IMG_Name(SEC_Img(RTN_Sec(rtn))).c_str());
    // rc->_address = RTN_Address(rtn);
    // rc->_icount = 0;
    // rc->_rtnCount = 0;

    RTN_Open(rtn);

    // Insert a call at the entry point of a routine to increment the call count
    // RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)docount, IARG_PTR,
    // &(rc->_rtnCount), IARG_END);

    // For each instruction of the routine
    // for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
    // {
    //     // Insert a call to docount to increment the instruction counter for
    //     this rtn INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount,
    //     IARG_PTR, &(rc->_icount), IARG_END);
    // }

    RTN_Close(rtn);
    // RTN_AddInstrumentFunction()
}

VOID Instruction(INS ins, VOID *v) {
    ADDRINT addr = INS_Address(ins);
    string insString = INS_Disassemble(ins);
    // fprintf(stderr, "addr:%lx, ins:%s\n", addr, insString.c_str());

    // Locate the crash_check call by the difference between the address of
    // the instruction and the target address
    if (INS_IsDirectControlFlow(ins) && INS_IsCall(ins) &&
        INS_DirectControlFlowTargetAddress(ins) - addr == 0xf17) {
        // fprintf(stderr, "found addr:%lx, ins:%s\n", addr, insString.c_str());
        retAddress = addr + 0xf17 + 0x107;
    }

    if (retAddress != 0 && addr == retAddress) {
        // fprintf(stderr, "found ret:%lx, ins:%s\n", addr, insString.c_str());
        INS_InsertCall(ins, IPOINT::IPOINT_BEFORE, (AFUNPTR)setReg,
                       IARG_REG_REFERENCE, REG_RAX, IARG_UINT64, 0x0, IARG_END);
    }

    if (insString == "mov eax, dword ptr [rip+0x26300]") {
        // fprintf(stderr, "found stakes:%lx, ins:%s\n", addr,
        // insString.c_str());
        INS_InsertCall(ins, IPOINT::IPOINT_AFTER, (AFUNPTR)setReg,
                       IARG_REG_REFERENCE, REG_RAX, IARG_UINT64, 5000,
                       IARG_END);
    }
    if (insString == "mov eax, dword ptr [rip+0x262e4]") {
        // fprintf(stderr, "found stakes:%lx, ins:%s\n", addr,
        // insString.c_str());
        INS_InsertCall(ins, IPOINT::IPOINT_AFTER, (AFUNPTR)setReg,
                       IARG_REG_REFERENCE, REG_RAX, IARG_UINT64, 5000,
                       IARG_END);
    }
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet
 * started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[]) {
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid
    if (PIN_Init(argc, argv)) {
        return Usage();
    }

    string fileName = KnobOutputFile.Value();

    if (!fileName.empty()) {
        out = new std::ofstream(fileName.c_str());
    }

    if (KnobCount) {
        // Register ImageLoad to be called when an image is loaded
        // IMG_AddInstrumentFunction(imageLoad, 0);
        // Register function to be called to instrument traces
        // TRACE_AddInstrumentFunction(Trace, 0);

        INS_AddInstrumentFunction(Instruction, 0);
        // RTN_AddInstrumentFunction(Routine, 0);

        // PIN_AddSyscallEntryFunction(SyscallEntry, 0);
        // PIN_AddSyscallExitFunction(SyscallExit, 0);

        // Register function to be called for every thread before it starts
        // running PIN_AddThreadStartFunction(ThreadStart, 0);

        // Register function to be called when the application exits
        // PIN_AddFiniFunction(Fini, 0);
    }

    cerr << "===============================================" << endl;
    cerr << "This application is instrumented by MyPinTool" << endl;
    if (!KnobOutputFile.Value().empty()) {
        cerr << "See file " << KnobOutputFile.Value() << " for analysis results"
             << endl;
    }
    cerr << "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
