#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Pass.h"
#include "Loger.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct Hello : public FunctionPass {
  static int numOfInstructions;
  static int numOfBlocks;
  static int numOfFunctions;

  static char ID;
  Hello() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    auto&& loger = Loger::create("NEWGRAPH.txt");

    CreateSubgraph(loger, F);
    for (auto &B : F) {
      int firstInstruction = numOfInstructions + 1;
      for (auto &I : B) {
        DumpInstruction(loger, I);
      }
      int lastInstruction  = numOfInstructions;
      ConnectInstructions(loger, firstInstruction, lastInstruction);
    }

    loger << "}\n";
    return false;
  }

  private:
    void CreateSubgraph(raw_fd_ostream& loger, const Function& F) {
      loger << "subgraph cluster_" << ++numOfFunctions << " {\n"
            << "node [style=filled];\n"
            << "label = \"" << F.getName() <<"\";\n"
            << "color = blue;\n";
    }

    void DumpInstruction(raw_fd_ostream& loger, const Instruction& I) {
      loger << "Instruction" << ++numOfInstructions << " [label=\"";
      I.print(loger);
      loger << "\"];\n";
    }

    void ConnectInstructions(raw_fd_ostream& loger, int begin, int end) {
      while (begin != end) {
        loger << "Instruction" << begin << " -> " << "Instruction" << begin + 1 << ";\n";
        ++begin;
      }
    }
}; // end of struct Hello

char Hello::ID = 0;
int  Hello::numOfInstructions = -1;
int  Hello::numOfFunctions = -1;
int  Hello::numOfBlocks = -1;

static RegisterPass<Hello> X("hello", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

}  // end of anonymous namespace