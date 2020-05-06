#include "llvm/Function.h"
#include "llvm/IRBuilder.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Type.h"

using namespace llvm;

namespace {
struct bishe_insert : public ModulePass {
    static char ID;
    Function *hook;

    bishe_insert() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
        Constant *hookFunc;
        hookFunc = M.getOrInsertFunction("print", Type::getVoidTy(M.getContext()), (Type *)0);

        hook = cast<Function>(hookFunc);

        for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
            for (Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
                bishe_insert::runOnBasicBlock(BB);
            }
        }

        return false;
    }
    virtual bool runOnBasicBlock(Function::iterator &BB) {
        for (BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
            if (isa<LoadInst>(&(*BI))) {
                LoadInst *CI = dyn_cast<LoadInst>(BI);
                Instruction *newInst = CallInst::Create(hook, "");
                BB->getInstList().insert((Instruction *)CI, newInst);
            }
        }
        return true;
    }
};
}  // namespace
char bishe_insert::ID = 0;
static RegisterPass<bishe_insert> X("bishe_insert", "test function exist", false, false);