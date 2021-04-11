gcc -std=c99 -g3 test.c -o test
gcc -std=c99 -g3 -O0 test.c -o test0
gcc -std=c99 -g3 -O1 test.c -o test1
gcc -std=c99 -g3 -O2 test.c -o test2
gcc -std=c99 -g3 -O3 test.c -o test3
objdump -W test > test.obj
objdump -W test0 > test0.obj
objdump -W test1 > test1.obj
objdump -W test2 > test2.obj
objdump -W test3 > test3.obj

