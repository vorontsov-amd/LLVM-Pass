#include "llvm/ADT/StringRef.h"
#include "Loger.hpp"
#include "llvm/Analysis/BlockFrequencyInfoImpl.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IRBuilderFolder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Use.h"
#include "llvm/Pass.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"

namespace {

using namespace llvm;

struct Hello : public FunctionPass {
  static char ID;
  Hello() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    if (isFuncLogger(F.getName())) {
      return false;
    }

    auto&& loger = Loger::create("NEWGRAPH.txt");

    CreateSubgraph(loger, F);
    for (auto &&B : F) {
      CreateSubgraph(loger, B);
      ConnectInstructions(loger, B);
      for (auto &&I : B) {
        DumpInstruction(loger, I);
        ConnectToUser(loger, I);
      }
      ConnectBasicBlock(loger, B);
      loger << "}\n";
    }
    loger << "}\n";

    // Prepare builder for IR modification
    LLVMContext &Ctx = F.getContext();
    IRBuilder<> builder(Ctx);

    auto&& binOptLogFunc32 = CreateLogFunc32(F, builder);
    auto&& binOptLogFunc64 = CreateLogFunc64(F, builder);
    CallLogFunc(F, builder, binOptLogFunc32, binOptLogFunc64);
    return true;
  }

  private:
    static void CreateSubgraph(raw_fd_ostream& loger, const Function& F) {
      loger << "subgraph cluster_" << &F << " {\n"
            << "node [style = filled];\n"
            << "label = \"" << F.getName() <<"\";\n"
            << "color = blue;\n";
    }

    static void CreateSubgraph(raw_fd_ostream& loger, const BasicBlock& B) {
      loger << "subgraph cluster_" << &B << " {\n"
            << "node [style = filled, color = white];\n"
            << "label = \"label ";
      B.printAsOperand(loger, false);
      loger <<"\";\n"
            << "color = lightgrey;\n"
            << "style = filled\n";
    }

    static void DumpInstruction(raw_fd_ostream& loger, const Instruction& I) {
      loger << "Instruction" << &I << " [label=\"";
      I.print(loger);
      loger << "\"];\n";
    }

    static void ConnectInstructions(raw_fd_ostream& loger, const BasicBlock& B) {
      for (auto &&it = B.begin(), &&next = std::next(it); next != B.end(); it = next, ++next) {
        loger << "Instruction" << &(*it) << " -> " << "Instruction" << &(*next)
              << "[color = blue, style = dotted, arrowhead = none];\n";
      }
    }

    static void ConnectToUser(raw_fd_ostream& loger, const Instruction& I) {
      for (auto &&U : I.uses()) {
        auto&& pUser = U.getUser();
        loger << "Instruction" << &I << " -> " << "Instruction" << pUser << "[label = \"";
        I.printAsOperand(loger, false);
        loger << "\"];\n";
      }
    }

    static void ConnectBasicBlock(raw_fd_ostream& loger, const BasicBlock& B) {
      for (auto &&U : B.uses()) {
        auto&& pUser = U.getUser();
        loger << "Instruction" << pUser << " -> " << "Instruction" << &(*B.begin()) << ";\n";
      }
    }

    static bool isFuncLogger(StringRef name) {
      return name == "binOptLogger32"  || name == "binOptLogger64";
    }

    static FunctionCallee CreateLogFunc32(Function& F, IRBuilder<>& builder) {
      std::vector<Type *> binOptParamTypes = {builder.getInt32Ty(),
                                              builder.getInt32Ty(),
                                              builder.getInt32Ty(),
                                              builder.getInt8Ty()->getPointerTo(),
                                              builder.getInt8Ty()->getPointerTo(),
                                              builder.getInt64Ty()};
      auto *binOptLogFuncType = FunctionType::get(builder.getVoidTy(), binOptParamTypes, false);
      return F.getParent()->getOrInsertFunction("binOptLogger32", binOptLogFuncType);
    }

    static FunctionCallee CreateLogFunc64(Function& F, IRBuilder<>& builder) {
      std::vector<Type *> binOptParamTypes = {builder.getInt64Ty(),
                                              builder.getInt64Ty(),
                                              builder.getInt64Ty(),
                                              builder.getInt8Ty()->getPointerTo(),
                                              builder.getInt8Ty()->getPointerTo(),
                                              builder.getInt64Ty()};
      auto *binOptLogFuncType = FunctionType::get(builder.getVoidTy(), binOptParamTypes, false);
      return F.getParent()->getOrInsertFunction("binOptLogger64", binOptLogFuncType);
    }

    static void CallLogFunc(Function& F, IRBuilder<>& builder, const FunctionCallee& binOptLogFunc32, const FunctionCallee& binOptLogFunc64) {
      for (auto &B : F) {
        for (auto &I : B) {
          Value *valueAddr = ConstantInt::get(builder.getInt64Ty(), reinterpret_cast<int64_t>(&I));
          if (auto *op = dyn_cast<BinaryOperator>(&I)) {
            // Insert after op
            builder.SetInsertPoint(op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
            // Insert a call to binOptLogFunc function
            Value *lhs = op->getOperand(0);
            Value *rhs = op->getOperand(1);
            Value *funcName = builder.CreateGlobalStringPtr(F.getName());
            Value *opName = builder.CreateGlobalStringPtr(op->getOpcodeName());
            Value *args[] = {op, lhs, rhs, opName, funcName, valueAddr};
            if      (op->getType() == builder.getInt32Ty()) builder.CreateCall(binOptLogFunc32, args);
            else if (op->getType() == builder.getInt64Ty()) builder.CreateCall(binOptLogFunc64, args);
          }
        }
      }
    }
}; // end of struct Hello

char Hello::ID = 0;

static RegisterPass<Hello> X("hello", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

}  // end of anonymous namespace