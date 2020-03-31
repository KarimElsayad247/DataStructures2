#ifndef FARRAY_H_INCLUDED
#define FARRAY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

size_t file_elemnts_number(FILE* target_file);

int* array_from_file(FILE* target_file, size_t array_size);

#endif // FARRAY_H_INCLUDED
