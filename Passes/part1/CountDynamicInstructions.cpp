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
struct CountDynamicInstructions : public FunctionPass {
  static char ID;
  Module* module;
  // Module* module;
  // Constant* lib231UpdateInstr;
  // Constant* lib231PrintInstr;
  //LLVMContext* context;
  CountDynamicInstructions() : FunctionPass(ID) {}

  bool doInitialization(Module &M) override {
    module = &M;
    // module = &M;
    // LLVMContext& context = module->getContext();
    //context = &(module->getContext()):
    // lib231UpdateInstr = M.getOrInsertFunction("updateInstrInfo",Type::getVoidTy(context)
    // ,Type::getInt32Ty(context),Type::getInt32Ptrty(context),Type::getInt32Ptrty(context));
    //
    // lib231PrintInstr = M.getOrInsertFunction("printOutBranchInfo",Type::getVoidTy(context));
    //LLVMContext &context = mod.getContext();
    //errs().write_escaped(F.getName()) << '\n';
    return false;
  }

  bool runOnFunction(Function &F) override {
    LLVMContext& context = module->getContext();
    Constant* lib231UpdateInstr = module->getOrInsertFunction("updateInstrInfo",Type::getVoidTy(context),Type::getInt32Ty(context),Type::getInt32PtrTy(context),Type::getInt32PtrTy(context));

    Constant* lib231PrintInstr = module->getOrInsertFunction("printOutInstrInfo",Type::getVoidTy(context));
    //errs() << F <<'\n';
    for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {
      std::map<uint32_t,uint32_t> myMap;
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        myMap[I->getOpcode()]++;
      }
      uint32_t size = myMap.size();
      IRBuilder<> irBuilder(&*B);
      irBuilder.SetInsertPoint(B->getTerminator());

      std::vector<uint32_t> keys;
      std::vector<uint32_t> values;
      for (auto it = myMap.begin(); it != myMap.end(); ++it){
        errs() <<"this map: " << it->first << '\t' << it->second << '\n';
        keys.push_back(it->first);
        values.push_back(it->second);
      }

      Constant * keysGlobal = ConstantDataArray::get(context, *(new ArrayRef<uint32_t>(keys)));
      Constant * valuesGlobal = ConstantDataArray::get(context, *(new ArrayRef<uint32_t>(values)));

      ArrayType* arrTy = ArrayType::get(IntegerType::get(context, 32), size);

      GlobalVariable* keys_global = new GlobalVariable(*module,arrTy,true,GlobalValue::InternalLinkage,keysGlobal,"keysGlobal");
      GlobalVariable* values_global = new GlobalVariable(*module,arrTy,true,GlobalValue::InternalLinkage,valuesGlobal,"valuesGlobal");

      std::vector<Value*> lib231Args;
      lib231Args.push_back(ConstantInt::get(Type::getInt32Ty(context), size));
      lib231Args.push_back(irBuilder.CreatePointerCast(keys_global, Type::getInt32PtrTy(context)));
      lib231Args.push_back(irBuilder.CreatePointerCast(values_global, Type::getInt32PtrTy(context)));

      irBuilder.CreateCall(lib231UpdateInstr,lib231Args);
      // if((string)((B->getTerminator())->getOpcodeName()) == "ret"){
      //   irBuilder.CreateCall(lib231PrintInstr);
      // }
      for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
        if ((string) I->getOpcodeName() == "ret") {
          errs() <<"reach ret"<<'\n';
          irBuilder.SetInsertPoint(&*I);
          irBuilder.CreateCall(lib231PrintInstr);
        }
      }
    }
    return false;
  }
}; // end of struct TestPass
}  // end of anonymous namespace

char CountDynamicInstructions::ID = 0;
static RegisterPass<CountDynamicInstructions> X("cse231-cdi", "Developed to test part2",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
