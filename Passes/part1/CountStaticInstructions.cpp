#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <map>

using namespace llvm;

namespace {
struct CountStaticinstructions : public FunctionPass {
  static char ID;
  CountStaticinstructions() : FunctionPass(ID) {}

  bool doInitialization(Module &) override {
    //LLVMContext &context = mod.getContext();
    //errs().write_escaped(F.getName()) << '\n';
    return false;
  }

  bool runOnFunction(Function &F) override {
    // struct cmp_str{
    //   bool operator()(const char* a, const char* b) const{
    //     return std::strcmp(a, b) < 0;
    //   }
    // };
    std::map<const char*,int> myMap;
    for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        myMap[I->getOpcodeName()]++;
      }
    }
    for (auto it = myMap.begin(); it != myMap.end(); ++it){
      errs() << it->first << '\t' << it->second << '\n';
    }
    return true;
  }
}; // end of struct TestPass
}  // end of anonymous namespace

char CountStaticinstructions::ID = 0;
static RegisterPass<CountStaticinstructions> X("cse231-csi", "Developed to test part1",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
