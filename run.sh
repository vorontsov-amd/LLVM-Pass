rm -rf log.txt
clang -O0 -emit-llvm src/hello.c -S -o src/hello.ll
cd ../llvm-project/llvm/build
make
opt -enable-new-pm=0 -load lib/LLVMHello.so -hello ../../../examples/src/hello.ll -o ../../../examples/hello
