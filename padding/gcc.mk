gcc -x c -std=c99 -O0 -fverbose-asm  -S main.cpp && cat main.s | grep offsetArray
