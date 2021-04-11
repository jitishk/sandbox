#include <stddef.h>

struct s {
        int i;
        char c;
        double d;
        char a[];
    };

int main()
{
    int offsetArray[4] = { offsetof(struct s, i ), offsetof( struct s, c ), offsetof(struct s, d ), offsetof(struct s, a )  } ;

    return 0 ;
}

/*

Make with
gcc -x c -std=c99 -O0 -fverbose-asm  -S main.cpp && cat main.s | grep offsetArray
Output
movl    $0, -16(%rbp)   #, offsetArray
movl    $4, -12(%rbp)   #, offsetArray
movl    $8, -8(%rbp)    #, offsetArray
movl    $16, -4(%rbp)   #, offsetArray

