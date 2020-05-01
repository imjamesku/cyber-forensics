clang++ -emit-llvm -o sample1.bc -c sample1.c
clang++ -emit-llvm -o sample2.bc -c sample2.c
clang++ -emit-llvm -o sample3.bc -c sample3.c
clang++ -emit-llvm -o sample4.bc -c sample4.c

llvm-dis ./sample1.bc
llvm-dis ./sample2.bc
llvm-dis ./sample3.bc
llvm-dis ./sample4.bc
