#include <stdio.h>
#include <string.h>

#define N 4
#define n 6
#define SWAP(a,b) {\
    int temp = a; \
    a = b; \
    b = temp; \
}

void
print_array(int *array)
{
    printf("[ ");
    for (int i=0; i<n; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");
}

void
init_array(int *array)
{
    memset(array, 0, n*sizeof(int));
}

void
row(int *array, int s, int e)
{
    array[s] = 1;
    for(int i=s; i<e; i++) {
        SWAP(array[s], array[i]);
        print_array(array);
        SWAP(array[i], array[s]);
    }
    array[s] = 0;
}

int main(int argc, const char *argv[])
{
    int array[n];
    init_array(array);
    //print_array(array);
    for (int i = 0; i < n-N+1; i++) {
        array[i] = 1;
        for (int j = i+1; j < n-N+2; j++) {
            array[j] = 1;
            for (int k = j+1; k < n-N+3; k++) {
                array[k] = 1;
                row(array, k+1, n-N+4);
                array[k] = 0;
            }
            array[j] = 0;
        }
        array[i] = 0;
    }
    return 0;
}
