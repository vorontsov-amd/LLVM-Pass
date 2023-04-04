# remove log file from the past run pass
rm -rf output
mkdir output

llvm_project=$1 # path to llvm-project
programm=$2     # path to test programm
current_path=$(pwd)

# add functions for logging to user code and compile it
cp ${programm}.c output/${programm}.c
cat src/log.c >> output/${programm}.c
clang -O0 -emit-llvm output/${programm}.c -c -o output/${programm}.bc

# Compile LLVM
cd $llvm_project/llvm/build
cmake --build .
opt -enable-new-pm=0 -load lib/llvmGraphvizPass.so -graphviz $current_path/output/${programm}.bc -o $current_path/output/${programm}_modificated.bc

# compile support programm for connect static and dynamic info
cd $current_path
clang++ src/ConnectDynInfo.cpp -o output/connect.out

# run programm
lli output/$2_modificated.bc $3

# move log file
mv $llvm_project/llvm/build/StaticGraph.txt output
mv ./log.txt output 

# link static and dynamic info
./output/connect.out

# create graphviz
dot output/StaticGraph.txt -T pdf -o output/StaticGraph.pdf
dot output/FinalGraph.txt -T pdf -o output/FinalGraph.pdf