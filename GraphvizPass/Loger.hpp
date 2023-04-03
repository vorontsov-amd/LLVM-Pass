#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <string>
#include <system_error>

class Loger {
  Loger(llvm::StringRef filename, std::error_code errcode) : stream(filename, errcode) {
    if (!stream.has_error()) {
      stream << "digraph LLVM {\n";
    }
  }

  llvm::raw_fd_ostream& getStream() {
    return stream;
  }

  Loger(const Loger&)               = delete;
  Loger(Loger&&)                    = delete;
  Loger& operator=(const Loger&)    = delete;
  Loger& operator=(Loger&&)         = delete;

  llvm::raw_fd_ostream stream;

  public:
    static llvm::raw_fd_ostream& create(llvm::StringRef filename) {
      std::error_code errc;
      static Loger loger (filename, errc);
      return loger.getStream();
    }

    ~Loger() {
      stream << "\n}";
      stream.close();
    }
};