#include <stdlib.h>
#include <string.h>
#include <assert.h>

void * ft_malloc (size_t size)
{
    void *memory = malloc(size);
    assert(memory != NULL);
    memset(memory, 0, size);
    return memory;
}

void ft_free (void **in_memory)
{
    void *memory = (*in_memory);
    free(memory);
    (*in_memory) = NULL;
}
