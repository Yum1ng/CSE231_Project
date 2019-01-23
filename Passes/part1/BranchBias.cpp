#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <vector>
#include <string>


using namespace llvm;
using namespace std;

namespace {
struct BranchBias : public FunctionPass {
  static char ID;
  Module* module;
  // Module* module;
  // Constant* lib231UpdateInstr;
  // Constant* lib231PrintInstr;
  //LLVMContext* context;
  BranchBias() : FunctionPass(ID) {}

  bool doInitialization(Module &M) override {
    module = &M;
    return false;
  }

  bool runOnFunction(Function &F) override {
    LLVMContext& context = module->getContext();
    Constant* lib231UpdateBranchInfo = module->getOrInsertFunction("updateBranchInfo",Type::getVoidTy(context),Type::getInt1Ty(context));
    Constant* lib231PrintOutBranchInfo = module->getOrInsertFunction("printOutBranchInfo",Type::getVoidTy(context));
    for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {
      IRBuilder<> irBuilder(&*B);
      irBuilder.SetInsertPoint(B->getTerminator());
      BranchInst *br = dyn_cast<BranchInst>(B -> getTerminator());
      if (br != NULL && br->isConditional()) {
        vector<Value *> args;
        args.push_back(br->getCondition());
        irBuilder.CreateCall(lib231UpdateBranchInfo, args);
      }
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        if ((string) I->getOpcodeName() == "ret") {
          irBuilder.SetInsertPoint(&*I);
          irBuilder.CreateCall(lib231PrintOutBranchInfo);
        }
      }
    }
    return false;
  }
}; // end of struct TestPass
}  // end of anonymous namespace

char BranchBias::ID = 0;
static RegisterPass<BranchBias> X("cse231-bb", "Developed to do part3",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
