
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

UINT64 insCount = 0;     // number of dynamically executed instructions
UINT64 bblCount = 0;     // number of dynamically executed basic blocks
UINT64 threadCount = 0;  // total number of threads, including main thread
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
    fprintf(stderr, "change reg to %lx\n", val);
    *regRef = val;
}

VOID loadDump(ADDRINT *rdxRef, ADDRINT *rcxRef) {
    // Remember to change the path accordingly
    FILE *f = fopen("memory-dump/2824-7fffbe15c000-7fffbe17d000.dump", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    printf("size: %ld\n", fsize);
    fseek(f, 0, SEEK_SET); /* same as rewind(f); */

    fread(buffer, 1, fsize, f);
    printf("buffer:%lx\n", (uint64_t)buffer);
    ADDRINT head = (ADDRINT)(buffer + (0x7fffbe15eaf0 - 0x7fffbe15c000));
    printf("head:%lx\n", head);
    int64_t *cur = (int64_t *)head;

    uint64_t len = 0;
    // Fix pointers in the buffer
    while (cur != NULL) {
        for (size_t i = 0; i < 8; i++) {
            *cur = *cur - 0x7fffbe15c000 + (int64_t)buffer;
            cur++;
        }
        len++;
        if (*cur != 0) {
            *cur = *cur - 0x7fffbe15c000 + (int64_t)buffer;
            cur = (int64_t *)*cur;
        } else {
            break;
        }
    }

    printf("total number of chars: %ld\n", len);
    // Overwrite rdx to use the loaded buffer as the argument
    *rdxRef = head;
    printf("reg after:%lx\n", *rdxRef);
    *rcxRef = len;
    fclose(f);
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

VOID Instruction(INS ins, VOID *v) {
    ADDRINT addr = INS_Address(ins);
    string insString = INS_Disassemble(ins);
    // fprintf(stderr, "addr:%lx, ins:%s\n", addr, insString.c_str());

    // Locate the stega_encrypt call by the difference between the address of the instruction and the target address
    if (INS_IsDirectControlFlow(ins) && INS_IsCall(ins) && addr - INS_DirectControlFlowTargetAddress(ins) == 0x4d0) {
        fprintf(stderr, "found addr:%lx, ins:%s\n", addr, insString.c_str());
        // Insert a callback to load the memory dump file. Do make sure the file path is correct.
        INS_InsertCall(ins, IPOINT::IPOINT_BEFORE, (AFUNPTR)loadDump, IARG_REG_REFERENCE, REG_DX, IARG_REG_REFERENCE, REG_CX, IARG_END);
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
        // PIN_AddSyscallEntryFunction(SyscallEntry, 0);
        // PIN_AddSyscallExitFunction(SyscallExit, 0);

        // Register function to be called for every thread before it starts running
        // PIN_AddThreadStartFunction(ThreadStart, 0);

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
