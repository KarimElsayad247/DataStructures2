#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "farray.h"



typedef int DATA;



void swap(DATA* array, int element1, int element2)
{
    DATA temp = array[element1];
    array[element1] = array[element2];
    array[element2] = temp;
}



void max_heapify(DATA* array,int heap_size,int element)
{
    //indexing starts at 0
    //compare children, and put the index of the biggest child in max_key
    int left    = element*2 +1;
    int right   = element*2 +2;
    int max_key = element;

    // if the left element exists and is larger than the parent, take that as max key
    if(left < heap_size && array[left]>array[max_key])
    {
        max_key = left;
    }

    //if the right element exists and is larger than the left element, take that as max
    if(right<heap_size && array[right]>array[max_key])
    {
        max_key = right;
    }


    if(max_key != element)
    {
        //swap the biggest child with its parent
        swap(array, element, max_key);

        //make sure the max property remains true recursively
        max_heapify(array,heap_size,max_key);
    }

}


void build_max_heap(DATA* array, int number_of_elements)
{
    //starting from the parent of the last element, build the heap
    for(int i = (number_of_elements-1)/2; i>=0; i--)
    {
        max_heapify(array, number_of_elements, i);
    }
}


//indexing starts at 0
void heap_sort(DATA* root, int heap_size)
{
    for(int i = heap_size-1; i>=0; i--)
    {
        swap(root, 0, i);

        max_heapify(root, i, 0);
    }
}


void print_array(int* array_to_print, size_t array_size)
{
    //arbitrary number: it takes too long to print an array
    if(array_size>10000)
    {
        printf("Array too big to print: time consuming\n");
        return;
    }
    for(size_t i = 0; i < array_size; i++)
    {
        printf("%4d", array_to_print[i]);
    }
    printf("\n");
}

int main(int argc, char** argv)
{

    //make sure a name is entered
    if(argc < 2)
    {
        printf("Please enter a file name!\nPress enter to exit");
        getchar();
        exit(-1);
    }
    //and no more than 2 arguments
    else if(argc > 2)
    {
        printf("Please enter only one name!\nPress enter to exit");
        getchar();
        exit(-1);
    }

    //open target file
    FILE* target_file = fopen((const char*)argv[1],"r");
    if(target_file == NULL)
    {
        printf("Error opening file! make sure the name is correct!\n");
        exit(-2);
    }

    //calculate number of elements in the file
    size_t array_size = file_elemnts_number(target_file);
    printf("number of elements %d\n", array_size);

    //read file into array
    int* array_to_sort = array_from_file(target_file, array_size);
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    build_max_heap(array_to_sort, array_size);
    heap_sort(array_to_sort, array_size);
    end = clock();
    cpu_time_used = ((double) (end - start))*1000.0 / CLOCKS_PER_SEC;
    printf("Sorted Array: \n");
    print_array(array_to_sort, array_size);
    printf("Time taken: %2.0f ms\n", cpu_time_used);

}
