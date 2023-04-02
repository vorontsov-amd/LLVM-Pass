#include "llvm/ADT/StringRef.h"
#include "Loger.hpp"
#include "llvm/Analysis/BlockFrequencyInfoImpl.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Use.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <iterator>
#include <string>

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
    return false;

//         // Dump Function
//         loger << "In a function called " << F.getName() << "\n";
//         F.print(loger);
//         loger << "\n";
//
//         // Dump function uses
//         for (auto &U : F.uses()) {
//           User *user = U.getUser();
//           loger << "[DOT] " << (uint64_t)(&F) << " -> " << (uint64_t)user << "\n";
//           loger << "    User:  " << (uint64_t)user;
//           user->print(loger, true);
//           loger << "\n";
//         }
//
//         for (auto &B : F) {
//           // Dump BasicBlocks
//           // loger << "Basic block:\n";
//           // B.print(llvm::loger);
//           // loger << "\n";
//           for (auto &I : B) {
//             // Dump Instructions
//             loger << "Instruction: " << (uint64_t)(&I) << "\n";
//             I.print(loger, true);
//             loger << "\n";
//             // Dump instruction uses
//             for (auto &U : I.uses()) {
//               User *user = U.getUser();
//               loger << "[DOT] " << (uint64_t)(&I) << " -> " << (uint64_t)user
//                      << "\n    User:  " << (uint64_t)user;
//               user->print(loger, true);
//               loger << "\n";
//             }
//           }
//         }
//
//         return false;
  }

  private:
    void CreateSubgraph(raw_fd_ostream& loger, const Function& F) {
      loger << "subgraph cluster_" << ++numOfFunctions << " {\n"
            << "node [style = filled];\n"
            << "label = \"" << F.getName() <<"\";\n"
            << "color = blue;\n";
    }

    void CreateSubgraph(raw_fd_ostream& loger, const BasicBlock& B) {
      ++numOfBlocks;
      loger << "subgraph cluster_" << numOfBlocks << " {\n"
            << "node [style = filled, color = white];\n"
            << "label = \"label ";
      B.printAsOperand(loger, false);
      loger <<"\";\n"
            << "color = lightgrey;\n"
            << "style = filled\n";
    }

    void DumpInstruction(raw_fd_ostream& loger, const Instruction& I) {
      loger << "Instruction" << &I << " [label=\"";
      I.print(loger);
      loger << "\"];\n";
    }

    void ConnectInstructions(raw_fd_ostream& loger, const BasicBlock& B) {
      for (auto &&it = B.begin(), &&next = std::next(it); next != B.end(); it = next, ++next) {
        loger << "Instruction" << &(*it) << " -> " << "Instruction" << &(*next)
              << "[color = blue, style = dotted, arrowhead = none];\n";
      }
    }

    void ConnectToUser(raw_fd_ostream& loger, const Instruction& I) {
      for (auto &&U : I.uses()) {
        auto&& pUser = U.getUser();
        loger << "Instruction" << &I << " -> " << "Instruction" << pUser << "[label = \"";
        I.printAsOperand(loger, false);
        loger << "\"];\n";
      }
    }

    void ConnectBasicBlock(raw_fd_ostream& loger, const BasicBlock& B) {
      for (auto &&U : B.uses()) {
        auto&& pUser = U.getUser();
        loger << "Instruction" << pUser << " -> " << "Instruction" << &(*B.begin()) << ";\n";
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