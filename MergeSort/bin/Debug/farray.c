#include "farray.h"

#include <stdio.h>
#include <stdlib.h>


size_t file_elemnts_number(FILE* target_file)
{
    size_t element_counter = 0;
    char c;
    for (c = getc(target_file); c != EOF; c = getc(target_file))
    {
        if (c == '\n') // Increment count if this character is newline
            {
                element_counter++;  //increment each time you find a new line
            }
    }
    rewind(target_file);
    return element_counter;
}


int* array_from_file(FILE* target_file, size_t array_size)
{
    //dynamically allocate a new array of specified size
    int* new_array = malloc(array_size*sizeof(int));
    for(size_t i = 0; i < array_size; i++)
    {
        fscanf(target_file, "%d\n", new_array+i);
    }
    return new_array;
}
