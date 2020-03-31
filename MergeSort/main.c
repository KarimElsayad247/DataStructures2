#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "farray.h"


#define INT_MAX +2147483647

void merge_with_no_breakout(int array[], size_t start, size_t mid, size_t end)
{
    size_t first_array_size = mid - start +1;   //size of sorted left array
    size_t second_array_size = end - mid;       //size of right sorted array

    int temp_array_1[first_array_size+1];         //array to hold left elements
    int temp_array_2[second_array_size+1];        //array to hold right elements


    for (size_t i = 0; i < first_array_size; i++)   //copy left to first array
    {
        temp_array_1[i] = array[start + i];         //middle is taken here
    }
    temp_array_1[first_array_size] = INT_MAX;
    for (size_t j = 0; j < second_array_size; j++)              //copy right to first array
    {
        temp_array_2[j] = array[mid + j + 1];      //we start after element mid
    }
    temp_array_2[second_array_size] = INT_MAX;

    size_t i = 0;
    size_t j = 0;
    //now sort
    for (size_t k = start; k<=end; k++)
    {
        if(temp_array_1[i] < temp_array_2[j])
        {
            array[k] = temp_array_1[i++];
        }
        else
        {
            array[k] = temp_array_2[j++];
        }
    }
}


void merge_sort(int array[], size_t start, size_t end)
{
    size_t mid = (start + end)/2;
    if(start < end)
    {
        merge_sort(array, start, mid); //sort left half
        merge_sort(array, mid+1, end); //sort right half
        merge_with_no_breakout(array, start, mid, end); //merge both halves
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
        printf("%6d", array_to_print[i]);
    }
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
    char* n = "afdf";
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

    printf("%d\n", array_to_sort[array_size]);
    //print_array(array_to_sort, array_size);

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //sort
    merge_sort(array_to_sort,0,array_size);
    end = clock();



    cpu_time_used = ((double) (end - start))*1000.0 / CLOCKS_PER_SEC;

    printf("Execution took %f\n", cpu_time_used);

    //print_array(array_to_sort, array_size);
}
