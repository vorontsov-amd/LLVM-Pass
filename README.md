# LLVM-Pass
This is an example of my LLVM pass which collects static and dynamic information and generates a pdf file with a graph of this information. 

## Requirements 

The following applications have to be installed:

1. CMake 3.2 version (or higher)
3. Clang (version >= 13) 
4. LLVM
5. Graphviz

Information on installing LLVM and compiling Pass can be found [here](https://llvm.org/docs/GettingStarted.html) and [here](https://llvm.org/docs/WritingAnLLVMPass.html#running-a-pass-with-opt).

## Installing

1. Clone the repository
2. Find the folder where you installed [llvm-project](https://github.com/llvm/llvm-project)
3. Copy the GraphvizPass folder to the "llvm-project/llvm/lib/Transforms" directory.
4. Add the line ``add_subdirectory(GraphvizPass)`` to CMakefile.txt, located at "llvm-project/llvm/lib/Transforms/CMakeLists.txt"

## Compiling 

For a simple compilation, you can place your C file in the directory where you cloned this repository and run bash-script

```shell
> ./run.sh path/to/llvm-project name/of/your/file/without/extension command_line_arguments
```

## Example

```shell
> ./run.sh /home/amadey/Программы/LLVM/llvm-project helloTest
```

#### Output:

```shell
...
[100%] Built target PipSqueak
[100%] Built target SecondLib
[100%] Built target llvm_gtest_main
[100%] Built target benchmark
[100%] Built target benchmark_main
[100%] Built target llvm-locstats
Hello, world!
```
![](https://sun9-50.userapi.com/impg/B4SfTiVW_Dz7LXyRMnjini7fWeb4ikKyenLhuA/NH2mVX_2xKo.jpg?size=1614x568&quality=96&sign=439bc81e2fa5ee3d4999334b78df6f32&type=album)